#include "Christofides_Solver.hpp"
#include "../../local_search/TwoOpt.hpp"
#include <lemon/christofides_tsp.h>

namespace MOTSP {
    Christofides_Solver::Christofides_Solver(const Instance & instance,
                                             unsigned seed,
                                             double timeLimit,
                                             unsigned maxNumSolutions,
                                             bool captureEnabled) :
        Solver::Solver(instance,
                       seed,
                       timeLimit,
                       maxNumSolutions,
                       captureEnabled) {
    }

    Christofides_Solver::Christofides_Solver(const Instance & instance) :
        Solver::Solver(instance) {}

    Christofides_Solver::Christofides_Solver() : Solver::Solver() {}

    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        Christofides_Solver::getAllIndividuals() const {
        return this->allIndividuals;
    }

    void Christofides_Solver::solve() {
        this->startTime = std::chrono::steady_clock::now();

        unsigned numObjectives = this->instance.getNumObjectives(),
                 numVertices   = this->instance.getNumVertices();
        std::vector<std::vector<std::vector<double>>> adjs =
            this->instance.getAdj();

        lemon::FullGraph g(numVertices);
        lemon::FullGraph::EdgeMap<double> cost(g, 0.0);
        std::vector<unsigned> cycle(numVertices);
        std::vector<std::vector<double>> adj(numVertices,
                std::vector<double>(numVertices, 0.0));
        std::vector<std::vector<unsigned>> improvedCycles;
        std::vector<Solution> solutions;
        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            individuals;
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        std::vector<double> key(numVertices - 1);
        std::vector<std::pair<unsigned, double>> permutation(numVertices - 1);
        std::vector<std::vector<double>> pareto;
        unsigned i = 0;
        std::vector<double> middle(numObjectives, 0.0), newMiddle;
        std::vector<std::vector<double>> others, newOthers;
        std::queue<std::pair<std::vector<double>, 
                   std::vector<std::vector<double>>>> Q;

        const auto solveWeight = [&](std::vector<double> weight) {
            // Constructs a solution using the Christofides algorithm
            for(lemon::FullGraph::EdgeIt e(g); e != lemon::INVALID; ++e) {
                unsigned u = g.id(g.u(e));
                unsigned v = g.id(g.v(e));

                cost[e] = 0.0;
                adj[u][v] = 0.0;
                adj[v][u] = 0.0;

                for(unsigned j = 0; j < numObjectives; j++) {
                    cost[e] += adjs[j][u][v] * weight[j];
                    adj[u][v] += adjs[j][u][v] * weight[j];
                    adj[v][u] += adjs[j][v][u] * weight[j];
                }
            }

            lemon::ChristofidesTsp<lemon::FullGraph::EdgeMap<double>>
                christofides(g, cost);

            christofides.run();

            for(unsigned j = 0; j < numVertices; j++) {
                cycle[j] = g.id(christofides.tourNodes()[j]);
            }

            cycle = TwoOpt::improve(
                    std::vector<std::vector<std::vector<double>>>(1, adj),
                    cycle,
                    std::numeric_limits<unsigned>::max(),
                    1).front();

            improvedCycles = TwoOpt::improve(
                    adjs,
                    cycle,
                    std::numeric_limits<unsigned>::max(),
                    this->maxNumSolutions);

            solutions.resize(improvedCycles.size());

            std::transform(improvedCycles.begin(),
                           improvedCycles.end(),
                           solutions.begin(),
                           [&](const std::vector<unsigned> & c) {
                                return Solution(instance, c);
                           });

            individuals.resize(solutions.size());

            std::transform(improvedCycles.begin(),
                           improvedCycles.end(),
                           solutions.begin(),
                           individuals.begin(),
                           [&](const auto & cycle, const auto & solution) {
                                for(unsigned j = 0;
                                    j < numVertices - 1;
                                    j++) {
                                    key[j] = distribution(this->rng);
                                }

                                std::sort(key.begin(), key.end());

                                std::transform(cycle.begin() + 1,
                                               cycle.end(),
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

                                return std::make_pair(solution.getCost(), key);
                           });

            this->allIndividuals.insert(this->allIndividuals.end(),
                                        individuals.begin(),
                                        individuals.end());

            if(this->updateIndividuals(individuals) && this->captureEnabled) {
                pareto.resize(this->individuals.size());
                std::transform(this->individuals.begin(),
                               this->individuals.end(),
                               pareto.begin(),
                               [](const auto & individual){
                                    return individual.first;
                               });
                this->capturedParetos.push_back(pareto);
                this->capturedTimes.push_back(this->getElapsedTime());
                this->capturedIterations.push_back(i);
            }
        };

        for(unsigned j = 0; 
            j < numObjectives && !this->areTerminationCriteriaMet(); 
            j++) {
            std::fill(middle.begin(), middle.end(), 0.0);
            middle[j] = 1.0;
            others.push_back(middle);
            solveWeight(middle);
        }
        std::fill(middle.begin(), middle.end(), 1.0/((double) numObjectives));

        Q.push(std::make_pair(middle, others));

        while(!this->areTerminationCriteriaMet()) {
            middle = Q.front().first;
            others = Q.front().second;

            Q.pop();

            solveWeight(middle);

            for(unsigned j = 0;
                j < others.size() && !this->areTerminationCriteriaMet();
                j++) {
                newMiddle = middle;
                newOthers.clear();
                newOthers.push_back(middle);

                for(unsigned k = 0;
                    k < others.size() && !this->areTerminationCriteriaMet();
                    k++) {
                    if(j != k) {
                        std::transform(newMiddle.begin(),
                                       newMiddle.end(),
                                       others[k].begin(),
                                       newMiddle.begin(),
                                       std::plus<double>());
                        newOthers.push_back(others[k]);
                    }
                }

                std::transform(newMiddle.begin(),
                               newMiddle.end(),
                               newMiddle.begin(),
                               [&](double a) {
                                    return a/numObjectives;
                               });

                Q.push(std::make_pair(newMiddle, newOthers));
            }
        }

        if(this->captureEnabled) {
            pareto.resize(this->individuals.size());
            std::transform(this->individuals.begin(),
                           this->individuals.end(),
                           pareto.begin(),
                           [](const auto & individual){
                                return individual.first;
                           });
            this->capturedParetos.push_back(pareto);
            this->capturedTimes.push_back(this->getElapsedTime());
            this->capturedIterations.push_back(i);
        }

        if(this->allIndividuals.size() > this->maxNumSolutions) {
            auto fronts = 
                BRKGA::Population::nonDominatedSort<std::vector<double>>(
                        this->allIndividuals,
                        this->senses);

            this->allIndividuals.clear();

            for(i = 0; i < fronts.size(); i++) {
                if(this->allIndividuals.size() + fronts[i].size() >
                        this->maxNumSolutions) {
                    BRKGA::Population::crowdingSort<std::vector<double>>(
                            fronts[i]);
                }

                this->allIndividuals.insert(this->allIndividuals.end(),
                                            fronts[i].begin(),
                                            fronts[i].end());

                if(this->allIndividuals.size() > this->maxNumSolutions) {
                    this->allIndividuals.resize(this->maxNumSolutions);
                    break;
                }
            }
        }

        this->solutions.resize(this->individuals.size());

        std::transform(this->individuals.begin(),
                       this->individuals.end(),
                       this->solutions.begin(),
                       [](const auto & individual) {
                            return Solution(individual.second,
                                            individual.first);
                       }
                );

        this->solvingTime = this->getElapsedTime();
    }

    void Christofides_Solver::write(std::ostream & os) const {
        Solver::write(os);
        os << "All solutions found: " << this->allIndividuals.size()
           << std::endl;
    }

    void Christofides_Solver::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if (ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void Christofides_Solver::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void Christofides_Solver::write() const {
        this->write(std::cout);
    }
}

