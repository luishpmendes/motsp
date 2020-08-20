#include "TwoOpt.hpp"
#include "../../solution/Solution.hpp"
#include <algorithm>
#include <cmath>
#include <list>

namespace MOTSP {
    std::vector<std::vector<unsigned>> TwoOpt::improve(
            const std::vector<std::vector<std::vector<double>>> & adj,
            const std::vector<unsigned> & cycle,
            unsigned maxNumImprovments,
            unsigned maxNumSolutions) {
        std::vector<std::pair<std::vector<unsigned>, std::vector<double>>> 
            nonDominatedSolutions;
        std::list<std::pair<std::vector<unsigned>, std::vector<double>>> 
            unexploredSolutions;
        std::vector<double> costs(adj.size(), 0.0);
        unsigned numObjectives = adj.size(),
                 numVertices = adj.front().size(),
                 numImprovments = 0;

        for(unsigned i = 0; i < numVertices; i++) {
            unsigned u = cycle[i],
                     v = cycle[(i + 1) % numVertices];
            for(unsigned j = 0; j < numObjectives; j++) {
                costs[j] += adj[j][u][v];
            }
        }

        unexploredSolutions.push_back(std::make_pair(cycle, costs));

        std::pair<std::vector<unsigned>, std::vector<double>> solution;
        std::vector<unsigned> currCycle;
        std::vector<double> currCosts;
        std::vector<std::pair<std::vector<unsigned>, std::vector<double>>> 
            newSolutions;
        std::vector<unsigned> newCycle;
        std::vector<double> newCosts;

        while(!unexploredSolutions.empty() && 
                numImprovments < maxNumImprovments) {
            solution = unexploredSolutions.front();
            unexploredSolutions.pop_front();

            currCycle = solution.first;
            currCosts = solution.second;

            bool isDominatedOrEqual = false;

            for(unsigned i = 0; i < numVertices - 1; i++) {
                unsigned u1 = currCycle[i],
                         v1 = currCycle[(i + 1) % numVertices];

                for(unsigned j = i + 2; j < numVertices; j++) {
                    unsigned u2 = currCycle[j],
                             v2 = currCycle[(j + 1) % numVertices];
                    newCycle = currCycle;
                    newCosts = currCosts;

                    reverse(newCycle.begin() + i + 1, newCycle.begin() + j + 1);

                    std::vector<unsigned>::iterator it = find(newCycle.begin(),
                                                              newCycle.end(),
                                                              0);
                    if(it != newCycle.end()) {
                        std::rotate(newCycle.begin(), it, newCycle.end());
                    }

                    if(newCycle[1] > newCycle.back()) {
                        std::reverse(newCycle.begin() + 1, newCycle.end());
                    }

                    for(unsigned l = 0; l < numObjectives; l++) {
                        newCosts[l] -= adj[l][u1][v1] + adj[l][u2][v2];
                        newCosts[l] += adj[l][u1][u2] + adj[l][v1][v2];
                    }

                    if(Solution::dominates(newCosts, currCosts) || 
                            std::equal(newCosts.begin(),
                                       newCosts.end(),
                                       currCosts.begin(),
                                       [](double a, double b) {
                                            return fabs(a - b) <
                                                std::numeric_limits<double>::epsilon();
                                       })) {
                        isDominatedOrEqual = false;

                        for(auto it = newSolutions.begin();
                                 it != newSolutions.end();) {
                            auto solution = *it;

                            if(Solution::dominates(newCosts, solution.second)) {
                                it = newSolutions.erase(it);
                            } else {
                                if(Solution::dominates(solution.second,
                                                       newCosts) || 
                                        std::equal(solution.second.begin(),
                                                   solution.second.end(),
                                                   newCosts.begin(),
                                                   [](double a, double b) {
                                                        return fabs(a - b) <
                                                            std::numeric_limits<double>::epsilon();
                                                   })) {
                                    isDominatedOrEqual = true;
                                    break;
                                }

                                it++;
                            }
                        }

                        if(!isDominatedOrEqual 
                                && newSolutions.size() < maxNumSolutions) {
                            newSolutions.push_back(std::make_pair(newCycle,
                                                                  newCosts));
                        }
                    }
                }
            }

            for(auto it1 = newSolutions.begin(); it1 != newSolutions.end();) {
                auto newSolution = *it1;
                isDominatedOrEqual = false;

                for(auto it2 = nonDominatedSolutions.begin();
                         it2 != nonDominatedSolutions.end();) {
                    auto solution = *it2;

                    if(Solution::dominates(newSolution.second,
                                           solution.second)) {
                        it2 = nonDominatedSolutions.erase(it2);
                    } else {
                        if(Solution::dominates(solution.second,
                                                newSolution.second) ||
                                std::equal(solution.second.begin(),
                                           solution.second.end(),
                                           newSolution.second.begin(),
                                           [](double a, double b) {
                                                return fabs(a - b) <
                                                    std::numeric_limits<double>::epsilon();
                                           })) {
                            isDominatedOrEqual = true;
                            break;
                        }

                        it2++;
                    }
                }

                if(isDominatedOrEqual) {
                    it1 = newSolutions.erase(it1);
                } else {
                    if(nonDominatedSolutions.size() < maxNumSolutions) {
                        nonDominatedSolutions.push_back(newSolution);
                    }
                    it1++;
                }
            }

            for(auto newSolution : newSolutions) {
                isDominatedOrEqual = false;

                for(auto it = unexploredSolutions.begin();
                         it != unexploredSolutions.end();) {
                    auto solution = *it;
                    if(Solution::dominates(newSolution.second,
                                           solution.second)) {
                        it = unexploredSolutions.erase(it);
                    } else {
                        if(Solution::dominates(solution.second,
                                               newSolution.second) ||
                                std::equal(solution.second.begin(),
                                           solution.second.end(),
                                           newSolution.second.begin(),
                                           [](double a, double b) {
                                                return fabs(a - b) <
                                                    std::numeric_limits<double>::epsilon();
                                           })) {
                            isDominatedOrEqual = true;
                            break;
                        }

                        it++;
                    }
                }

                if(!isDominatedOrEqual 
                        && unexploredSolutions.size() < maxNumSolutions) {
                    unexploredSolutions.push_back(newSolution);
                }
            }

            numImprovments++;
        }

        std::vector<std::vector<unsigned>> result(nonDominatedSolutions.size());

        std::transform(nonDominatedSolutions.begin(),
                       nonDominatedSolutions.end(),
                       result.begin(),
                       [](const auto & solution) {
                            return solution.first;
                       });

        return result;
    }
}

