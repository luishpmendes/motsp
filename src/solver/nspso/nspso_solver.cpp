#include "solver/nspso/nspso_solver.hpp"
#include "solver/nspso/problem.hpp"
#include <pagmo/algorithms/nspso.hpp>

namespace motsp {

NSPSO_Solver::NSPSO_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

bool NSPSO_Solver::update_best_individuals(const pagmo::population & pop) {
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        new_individuals(pop.size());

    for(unsigned i = 0; i < pop.size(); i++) {
        new_individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
    }

    return Solver::update_best_individuals(new_individuals);
}

void NSPSO_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    pagmo::problem prob{Problem(this->instance)};

    pagmo::algorithm algo{pagmo::nspso(1,
                                       this->omega,
                                       this->c1,
                                       this->c2,
                                       this->chi,
                                       this->v_coeff,
                                       this->leader_selection_range,
                                       this->diversity_mechanism,
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

std::ostream & operator <<(std::ostream & os, const NSPSO_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Population size: " << solver.population_size << std::endl
       << "Omega: " << solver.omega << std::endl
       << "C1: " << solver.c1 << std::endl
       << "C2: " << solver.c2 << std::endl
       << "Chi: "  << solver.chi << std::endl
       << "vCoeff: " << solver.v_coeff << std::endl
       << "Leader selection range: " << solver.leader_selection_range
       << std::endl
       << "Diversity mechanism: " << solver.diversity_mechanism << std::endl
       << "Memory: " << solver.memory << std::endl;
    return os;
}

}

