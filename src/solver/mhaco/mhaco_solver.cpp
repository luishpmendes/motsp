#include "solver/mhaco/mhaco_solver.hpp"
#include "solver/mhaco/problem.hpp"
#include <pagmo/algorithms/maco.hpp>

namespace motsp {

MHACO_Solver::MHACO_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

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

