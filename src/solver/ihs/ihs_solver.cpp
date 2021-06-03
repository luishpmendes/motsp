#include "solver/ihs/ihs_solver.hpp"
#include "solver/ihs/problem.hpp"
#include <pagmo/algorithms/ihs.hpp>

namespace motsp {

IHS_Solver::IHS_Solver(const Instance & instance) : Solver::Solver(instance) {}

bool IHS_Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return Solver::update_best_individuals(new_individuals);
}

void IHS_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    pagmo::problem prob{Problem(this->instance)};

    pagmo::algorithm algo{pagmo::ihs(1,
                                     this->phmcr,
                                     this->ppar_min,
                                     this->ppar_max,
                                     this->bw_min,
                                     this->bw_max,
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

std::ostream & operator <<(std::ostream & os, const IHS_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Population size: " << solver.population_size << std::endl
       << "Probability of choosing from memory: " << solver.phmcr << std::endl
       << "Minimum pitch adjustment rate: " << solver.ppar_min << std::endl
       << "Maximum pitch adjustment rate: " << solver.ppar_max << std::endl
       << "Minimum distance bandwidth: " << solver.bw_min << std::endl
       << "Maximum distance bandwidth: " << solver.bw_max << std::endl;
    return os;
}

}

