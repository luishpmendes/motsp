#include "solver/solver.hpp"

namespace motsp {

Solver::Solver(const Instance & instance)
    : instance(instance),
      senses(instance.num_objectives, BRKGA::Sense::MINIMIZE) {
    this->set_seed(this->seed);
}

void Solver::set_seed(unsigned seed) {
    this->seed = seed;
    this->rng.seed(this->seed);
    this->rng.discard(10000);
}

double Solver::elapsed_time(
        const std::chrono::steady_clock::time_point & start_time) {
    std::chrono::steady_clock::time_point current_time =
        std::chrono::steady_clock::now();
    std::chrono::nanoseconds elapsed_time =
        std::chrono::duration_cast<std::chrono::nanoseconds> (
                current_time - start_time);
    return (double(elapsed_time.count()))/(double(1e9));
}

double Solver::elapsed_time() const {
    return Solver::elapsed_time(this->start_time);
}

double Solver::remaining_time(
        const std::chrono::steady_clock::time_point & start_time,
        double time_limit) {
    return time_limit - Solver::elapsed_time(start_time);
}

double Solver::remaining_time() const {
    return Solver::remaining_time(this->start_time, this->time_limit);
}

bool Solver::are_termination_criteria_met() const {
    return (this->elapsed_time() >= this->time_limit ||
            this->num_iterations >= this->iterations_limit);
}

bool Solver::update_best_individuals(
            const std::vector<
                std::pair<std::vector<double>,
                          std::vector<double>>> & new_individuals) {
    bool result = false;

    if(new_individuals.empty()) {
        return result;
    }

    auto non_dominated_new_individuals =
        BRKGA::Population::nonDominatedSort<std::vector<double>>(
                new_individuals,
                this->senses).front();

    for(const auto & new_individual : non_dominated_new_individuals) {
        bool is_dominated_or_equal = false;

        for(auto it  = this->best_individuals.begin();
                 it != this->best_individuals.end();) {
            auto individual = *it;

            if(Solution::dominates(new_individual.first, individual.first)) {
                it = this->best_individuals.erase(it);
            } else {
                if(Solution::dominates(individual.first,
                                       new_individual.first)
                        || std::equal(individual.first.begin(),
                                      individual.first.end(),
                                      new_individual.first.begin(),
                                      [](const double & a, const double & b) {
                                            return fabs(a - b) <
                                                std::numeric_limits<double>::epsilon();
                                      })) {
                    is_dominated_or_equal = true;
                    break;
                }

                it++;
            }
        }

        if(!is_dominated_or_equal) {
            this->best_individuals.push_back(new_individual);
            result = true;
        }
    }

    if(this->best_individuals.size() > this->max_num_solutions) {
        BRKGA::Population::crowdingSort<std::vector<double>>(
                this->best_individuals);
        this->best_individuals.resize(this->max_num_solutions);
        result = true;
    }

    return result;
}

bool Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return this->update_best_individuals(new_individuals);
}

void Solver::capture_snapshot(const pagmo::population & pop) {
    double time_snapshot = this->elapsed_time();

    this->pareto.resize(this->best_individuals.size());
    std::transform(this->best_individuals.begin(),
                   this->best_individuals.end(),
                   this->pareto.begin(),
                   [](const auto & individual) {
                        return individual.first;
                   });
    this->pareto_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                     time_snapshot,
                                                     this->pareto));

    this->current_individuals.resize(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        this->current_individuals[i] = std::make_pair(pop.get_f()[i],
                                                      pop.get_x()[i]);
    }

    this->fronts = BRKGA::Population::nonDominatedSort<std::vector<double>>(
            current_individuals,
            this->senses);

    this->non_dominated_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.front().size())));

    this->fronts_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.size())));

    this->time_last_snapshot = time_snapshot;
    this->iteration_last_snapshot = this->num_iterations;
    this->num_snapshots++;
}

std::ostream & operator <<(std::ostream & os, const Solver & solver) {
    os << "Number of objectives: " << solver.instance.num_objectives
       << std::endl
       << "Number of vertices: " << solver.instance.num_vertices << std::endl
       << "Seed: " << solver.seed << std::endl
       << "Time limit: " << solver.time_limit << std::endl
       << "Iterations limit: " << solver.iterations_limit << std::endl
       << "Initial individuals: " << solver.initial_individuals.size()
       << std::endl
       << "Maximum number of solutions: " << solver.max_num_solutions
       << std::endl
       << "Maximum number of snapshots: " << solver.max_num_snapshots
       << std::endl
       << "Number of iterations: " << solver.num_iterations << std::endl
       << "Solutions obtained: " << solver.best_solutions.size() << std::endl
       << "Solving time: " << solver.solving_time << std::endl
       << "Number of snapshots: " << solver.num_snapshots << std::endl;
    return os;
}

}

