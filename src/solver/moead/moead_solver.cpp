#include "solver/moead/moead_solver.hpp"
#include "solver/moead/problem.hpp"
#include <pagmo/algorithms/moead.hpp>

namespace motsp {
MOEAD_Solver::MOEAD_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

bool MOEAD_Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return Solver::update_best_individuals(new_individuals);
}

void MOEAD_Solver::capture_snapshot(const pagmo::population & pop) {
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

void MOEAD_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    pagmo::problem prob{Problem(this->instance)};

    pagmo::algorithm algo{pagmo::moead(1,
                                       this->weight_generation,
                                       this->decomposition,
                                       this->neighbours,
                                       this->cr,
                                       this->f,
                                       this->eta_m,
                                       this->realb,
                                       this->limit,
                                       this->preserve_diversity,
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

std::ostream & operator <<(std::ostream & os, const MOEAD_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Population size: " << solver.population_size << std::endl
       << "Method used to generate the weights: " << solver.weight_generation
       << std::endl
       << "Decomposition method: " << solver.decomposition << std::endl
       << "Size of the weight’s neighborhood: " << solver.neighbours
       << std::endl
       << "Crossover parameter in the Differential Evolution operator: "
       << solver.cr << std::endl
       << "Parameter for the Differential Evolution operator: " << solver.f
       << std::endl
       << "Chance that the neighbourhood is considered at each generation, "
       << "rather than the whole population: " << solver.realb << std::endl
       << "Maximum number of copies reinserted in the population: "
       << solver.limit << std::endl
       << "Diversity preservation mechanisms activated: "
       << solver.preserve_diversity << std::endl;
    return os;
}

}

