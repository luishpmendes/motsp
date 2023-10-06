#include "solver/moead/moead_solver.hpp"
#include "solver/moead/problem.hpp"
#include <pagmo/algorithms/moead.hpp>

namespace motsp {

MOEAD_Solver::MOEAD_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

MOEAD_Solver::MOEAD_Solver() = default;

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

    pagmo::population pop{
        prob,
        this->population_size - this->initial_individuals.size(),
        this->seed};

    for(const auto & individual : this->initial_individuals) {
        pop.push_back(individual.second, individual.first);
    }

    this->update_best_individuals(pop);

    if(this->max_num_snapshots > this->num_snapshots + 1) {
        this->capture_snapshot(pop);

        if (this->time_limit < std::numeric_limits<double>::max()) {
            this->time_snapshot_factor = std::pow(this->time_limit / this->time_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
            this->time_next_snapshot = this->time_last_snapshot * this->time_snapshot_factor;
        } else {
            this->time_next_snapshot = std::numeric_limits<double>::max();
            this->time_snapshot_factor = 1.0;
        }

        if (this->iterations_limit < std::numeric_limits<unsigned>::max()) {
            this->iteration_snapshot_factor = std::pow(this->iterations_limit / (this->iteration_last_snapshot + 1.0), 1.0 / (this->max_num_snapshots - this->num_snapshots));
            this->iteration_next_snapshot = unsigned(std::round(double(this->iteration_last_snapshot) * this->iteration_snapshot_factor));
        } else {
            this->iteration_next_snapshot = std::numeric_limits<unsigned>::max();
            this->iteration_snapshot_factor = 1.0;
        }
    } else {
        this->time_next_snapshot = 0.0;
        this->iteration_next_snapshot = 0;
        this->time_snapshot_factor = 1.0;
        this->iteration_snapshot_factor = 1.0;
    }

    while(!this->are_termination_criteria_met()) {
        this->num_iterations++;
        pop = algo.evolve(pop);
        this->update_best_individuals(pop);

        if(this->max_num_snapshots > this->num_snapshots + 1) {
            if (this->num_iterations >= this->iteration_next_snapshot) {
                this->capture_snapshot(pop);

                if (this->time_limit < std::numeric_limits<double>::max()) {
                    this->time_next_snapshot = this->time_last_snapshot * this->time_snapshot_factor;
                    this->time_snapshot_factor = std::pow(this->time_limit / this->time_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                }

                if (this->iterations_limit < std::numeric_limits<unsigned>::max()) {
                    this->iteration_next_snapshot = unsigned(std::round(double(this->iteration_last_snapshot) * this->iteration_snapshot_factor));
                    this->iteration_snapshot_factor = std::pow(this->iterations_limit / this->iteration_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                }
            } else if (this->elapsed_time() >= this->time_next_snapshot) {
                this->capture_snapshot(pop);

                if (this->time_limit < std::numeric_limits<double>::max()) {
                    this->time_next_snapshot = this->time_last_snapshot * this->time_snapshot_factor;
                    this->time_snapshot_factor = std::pow(this->time_limit / this->time_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                }

                if (this->iterations_limit < std::numeric_limits<unsigned>::max()) {
                    this->iteration_next_snapshot = unsigned(std::round(double(this->iteration_last_snapshot) * this->iteration_snapshot_factor));
                    this->iteration_snapshot_factor = std::pow(this->iterations_limit / this->iteration_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                }
            }
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
