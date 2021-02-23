#include "solver/mhaco/mhaco_solver.hpp"
#include "solver/mhaco/problem.hpp"
#include <pagmo/algorithms/maco.hpp>

namespace motsp {

MHACO_Solver::MHACO_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

bool MHACO_Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return Solver::update_best_individuals(new_individuals);
}

void MHACO_Solver::capture_snapshot(const pagmo::population & pop) {
    this->pareto.resize(this->best_individuals.size());
    std::transform(this->best_individuals.begin(),
                   this->best_individuals.end(),
                   this->pareto.begin(),
                   [](const auto & individual) {
                        return individual.first;
                   });
    this->pareto_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                     this->elapsed_time(),
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
                this->elapsed_time(),
                std::vector<unsigned>(1, fronts.front().size())));

    this->fronts_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                this->elapsed_time(),
                std::vector<unsigned>(1, fronts.size())));

    this->time_last_snapshot = this->elapsed_time();
    this->iteration_last_snapshot = this->num_iterations;
    this->num_snapshots++;
}

void MHACO_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    pagmo::problem prob{Problem(this->instance)};

    pagmo::algorithm algo{pagmo::maco(1,
                                      this->ker,
                                      this->q,
                                      this->threshold,
                                      this->n_gen_mark,
                                      this->eval_stop,
                                      this->focus,
                                      this->memory,
                                      this->seed)};

    if(this->max_num_snapshots > 0) {
        this->time_between_snapshots = this->time_limit /
            this->max_num_snapshots;
        this->iterations_between_snapshots = this->iterations_limit /
            this->max_num_snapshots;
    }

    pagmo::population pop{
        prob,
        this->population_size - this->initial_individuals.size(),
        this->seed};

    for(const auto & individual : this->initial_individuals) {
        pop.push_back(individual.second, individual.first);
    }

    this->update_best_individuals(pop);

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot(pop);
    }

    while(!this->are_termination_criteria_met()) {
        this->num_iterations++;
        pop = algo.evolve(pop);
        this->update_best_individuals(pop);

        if(this->max_num_snapshots > 0 &&
           this->num_snapshots < this->max_num_snapshots &&
          (this->elapsed_time() - this->time_last_snapshot >=
           this->time_between_snapshots ||
           this->num_iterations - this->iteration_last_snapshot >=
           this->iterations_between_snapshots)) {
            this->capture_snapshot(pop);
        }
    }

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot(pop);
    }

    this->best_solutions.clear();

    for(const auto & best_individual : this->best_individuals) {
        this->best_solutions.push_back(Solution(this->instance,
                                                best_individual.second));
    }

    this->solving_time = this->elapsed_time();
}

std::ostream & operator <<(std::ostream & os, const MHACO_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Population size: " << solver.population_size << std::endl
       << "Number of solutions stored in the solution archive: " << solver.ker
       << std::endl
       << "Convergence speed: " << solver.q << std::endl
       << "Threshold: " << solver.threshold << std::endl
       << "nGenMark: " << solver.n_gen_mark << std::endl
       << "EvalStop: " << solver.eval_stop << std::endl
       << "Focus: " << solver.focus << std::endl
       << "Memory: " << solver.memory << std::endl;
    return os;
}

}

