#include "christofides_solver.hpp"
#include "solver/local_search/two_opt.hpp"
#include <lemon/christofides_tsp.h>

namespace motsp {

Christofides_Solver::Christofides_Solver(const Instance & instance) :
    Solver::Solver(instance) {}

Christofides_Solver::Christofides_Solver() = default;

void Christofides_Solver::capture_snapshot() {
    double time_snapshot = this->elapsed_time();

    this->best_solutions_snapshots.emplace_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<std::vector<double>>(
                    this->best_individuals.size())));
    for(unsigned i = 0; i < this->best_individuals.size(); i++) {
        std::get<2>(this->best_solutions_snapshots.back())[i] =
            this->best_individuals[i].first;
    }

    this->fronts = NSBRKGA::Population::nonDominatedSort<std::vector<double>>(
            all_individuals,
            this->instance.senses);

    this->num_non_dominated_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.front().size())));

    this->num_fronts_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                time_snapshot,
                std::vector<unsigned>(1, fronts.size())));

    this->time_last_snapshot = time_snapshot;
    this->iteration_last_snapshot = this->num_iterations;
    this->num_snapshots++;
}

void Christofides_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    unsigned num_objectives = this->instance.num_objectives,
             num_vertices   = this->instance.num_vertices;
    std::vector<std::vector<std::vector<double>>> adjs = this->instance.adj;

    lemon::FullGraph g(num_vertices);
    lemon::FullGraph::EdgeMap<double> cost(g, 0.0);
    std::vector<unsigned> cycle(num_vertices);
    std::vector<std::vector<double>> adj(
            num_vertices,
            std::vector<double>(num_vertices, 0.0));
    std::vector<std::vector<unsigned>> improved_cycles;
    std::vector<Solution> solutions;
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        individuals;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::vector<double> key(num_vertices - 1);
    std::vector<std::pair<unsigned, double>> permutation(num_vertices - 1);
    std::vector<std::vector<double>> pareto;
    std::vector<double> middle(num_objectives, 0.0), new_middle;
    std::vector<std::vector<double>> others, new_others;
    std::queue<std::pair<std::vector<double>,
                         std::vector<std::vector<double>>>> Q;

    if(this->max_num_snapshots > this->num_snapshots + 1) {
        if (this->time_limit > 0.0) {
            this->time_snapshot_factor = std::pow(this->time_limit / this->elapsed_time(), 1.0 / (this->max_num_snapshots - this->num_snapshots));
            this->time_next_snapshot = this->elapsed_time() * this->time_snapshot_factor;
        } else {
            this->time_next_snapshot = 0.0;
            this->time_snapshot_factor = 1.0;
        }

        if (this->iterations_limit > 0) {
            this->iteration_snapshot_factor = std::pow(this->iterations_limit / 1, 1.0 / (this->max_num_snapshots - this->num_snapshots));
            this->iteration_next_snapshot = unsigned(std::round(1.0 * this->iteration_snapshot_factor));
        } else {
            this->iteration_next_snapshot = 0;
            this->iteration_snapshot_factor = 1.0;
        }
    } else {
        this->time_next_snapshot = 0.0;
        this->iteration_next_snapshot = 0;
        this->time_snapshot_factor = 1.0;
        this->iteration_snapshot_factor = 1.0;
    }

    const auto solve_weight = [&](std::vector<double> weight) {
        // Constructs a solution using the Christofides algorithm
        for(lemon::FullGraph::EdgeIt e(g); e != lemon::INVALID; ++e) {
            unsigned u = g.id(g.u(e));
            unsigned v = g.id(g.v(e));

            cost[e] = 0.0;
            adj[u][v] = 0.0;
            adj[v][u] = 0.0;

            for(unsigned j = 0; j < num_objectives; j++) {
                cost[e] += adjs[j][u][v] * weight[j];
                adj[u][v] += adjs[j][u][v] * weight[j];
                adj[v][u] += adjs[j][v][u] * weight[j];
            }
        }

        lemon::ChristofidesTsp<lemon::FullGraph::EdgeMap<double>>christofides(
                g, cost);

        christofides.run();

        for(unsigned j = 0; j < num_vertices; j++) {
            cycle[j] = g.id(christofides.tourNodes()[j]);
        }

        cycle = Two_Opt::improve(
                std::vector<std::vector<std::vector<double>>>(1, adj),
                cycle,
                std::numeric_limits<unsigned>::max(),
                1).front();

        improved_cycles = Two_Opt::improve(adjs,
                                           cycle,
                                           std::numeric_limits<unsigned>::max(),
                                           this->max_num_solutions);

        solutions.clear();

        for(const auto & improved_cycle : improved_cycles) {
            solutions.push_back(Solution(instance, improved_cycle));
        }

        individuals.resize(solutions.size());

        std::transform(improved_cycles.begin(),
                       improved_cycles.end(),
                       solutions.begin(),
                       individuals.begin(),
                       [&](const auto & improved_cycle, const auto & solution) {
                            for(unsigned j = 0;
                                j < num_vertices - 1;
                                j++) {
                                key[j] = distribution(this->rng);
                            }

                            std::sort(key.begin(), key.end());

                            std::transform(improved_cycle.begin() + 1,
                                           improved_cycle.end(),
                                           key.begin(),
                                           permutation.begin(),
                                           [](const auto & c,
                                              const auto & k) {
                                                return std::make_pair(c, k);
                                           });

                            std::sort(permutation.begin(),
                                      permutation.end());

                            std::transform(permutation.begin(),
                                           permutation.end(),
                                           key.begin(),
                                           [](const auto & p) {
                                                return p.second;
                                           });

                            return std::make_pair(solution.cost, key);
                       });

        this->all_individuals.insert(this->all_individuals.end(),
                                     individuals.begin(),
                                     individuals.end());

        this->update_best_individuals(individuals);

        if(this->max_num_snapshots > this->num_snapshots + 1) {
            if (this->iteration_next_snapshot > 0 && 
                this->num_iterations >= this->iteration_next_snapshot) {
                this->capture_snapshot();

                if (this->time_limit > 0.0) {
                    this->time_next_snapshot = this->time_last_snapshot * this->time_snapshot_factor;
                    this->time_snapshot_factor = std::pow(this->time_limit / this->time_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                } else {
                    this->time_next_snapshot = 0.0;
                    this->time_snapshot_factor = 1.0;
                }

                if (this->iterations_limit > 0) {
                    this->iteration_next_snapshot = unsigned(std::round(double(this->iteration_last_snapshot) * this->iteration_snapshot_factor));
                    this->iteration_snapshot_factor = std::pow(this->iterations_limit / this->iteration_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                } else {
                    this->iteration_next_snapshot = 0;
                    this->iteration_snapshot_factor = 1.0;
                }
            } else if (this->time_next_snapshot > 0.0 && 
                this->elapsed_time() >= this->time_next_snapshot) {
                this->capture_snapshot();

                if (this->time_limit > 0.0) {
                    this->time_next_snapshot = this->time_last_snapshot * this->time_snapshot_factor;
                    this->time_snapshot_factor = std::pow(this->time_limit / this->time_last_snapshot, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                } else {
                    this->time_next_snapshot = 0.0;
                    this->time_snapshot_factor = 1.0;
                }

                if (this->iterations_limit > 0) {
                    this->iteration_next_snapshot = unsigned(std::round(double(this->iteration_last_snapshot >= 1 ? this->iteration_last_snapshot : 1 ) * this->iteration_snapshot_factor));
                    this->iteration_snapshot_factor = std::pow(this->iterations_limit / this->iteration_last_snapshot >= 1 ? this->iteration_last_snapshot : 1, 1.0 / (this->max_num_snapshots - this->num_snapshots));
                } else {
                    this->iteration_next_snapshot = 0;
                    this->iteration_snapshot_factor = 1.0;
                }
            }
        }

        this->num_iterations++;
    };

    for(unsigned j = 0;
        j < num_objectives && !this->are_termination_criteria_met();
        j++) {
        std::fill(middle.begin(), middle.end(), 0.0);
        middle[j] = 1.0;
        others.push_back(middle);
        solve_weight(middle);
    }
    std::fill(middle.begin(), middle.end(), 1.0/((double) num_objectives));

    Q.push(std::make_pair(middle, others));

    while(!this->are_termination_criteria_met()) {
        middle = Q.front().first;
        others = Q.front().second;

        Q.pop();

        solve_weight(middle);

        for(unsigned j = 0;
            j < others.size() && !this->are_termination_criteria_met();
            j++) {
            new_middle = middle;
            new_others.clear();
            new_others.push_back(middle);

            for(unsigned k = 0;
                k < others.size() && !this->are_termination_criteria_met();
                k++) {
                if(j != k) {
                    std::transform(new_middle.begin(),
                                   new_middle.end(),
                                   others[k].begin(),
                                   new_middle.begin(),
                                   std::plus<double>());
                    new_others.push_back(others[k]);
                }
            }

            std::transform(new_middle.begin(),
                           new_middle.end(),
                           new_middle.begin(),
                           [&](double a) {
                                return a/num_objectives;
                           });

            Q.push(std::make_pair(new_middle, new_others));
        }
    }

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot();
    }

    if(this->all_individuals.size() > this->max_num_solutions) {
        auto fronts =
            NSBRKGA::Population::nonDominatedSort<std::vector<double>>(
                    this->all_individuals,
                    this->instance.senses);

        this->all_individuals.clear();

        for(unsigned i = 0; i < fronts.size(); i++) {
            if(this->all_individuals.size() + fronts[i].size() >
                    this->max_num_solutions) {
                NSBRKGA::Population::crowdingSort<std::vector<double>>(
                        fronts[i]);
            }

            this->all_individuals.insert(this->all_individuals.end(),
                                         fronts[i].begin(),
                                         fronts[i].end());

            if(this->all_individuals.size() > this->max_num_solutions) {
                this->all_individuals.resize(this->max_num_solutions);
                break;
            }
        }
    }

    this->best_solutions.clear();

    for(const auto & best_individual : this->best_individuals) {
        this->best_solutions.push_back(Solution(this->instance,
                                                best_individual.second));
    }

    this->solving_time = this->elapsed_time();
}

std::ostream & operator <<(std::ostream & os,
                           const Christofides_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "All solutions found: " << solver.all_individuals.size() << std::endl;
    return os;
}

}

