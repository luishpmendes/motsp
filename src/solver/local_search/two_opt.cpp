#include "two_opt.hpp"
#include "solution/solution.hpp"
#include <algorithm>
#include <cmath>
#include <list>

namespace motsp {

std::vector<std::vector<unsigned>> Two_Opt::improve(
        const std::vector<std::vector<std::vector<double>>> & adj,
        const std::vector<unsigned> & cycle,
        unsigned max_num_improvements,
        unsigned max_num_solutions) {
    std::vector<std::pair<std::vector<unsigned>, std::vector<double>>>
        non_dominated_solutions;
    std::list<std::pair<std::vector<unsigned>, std::vector<double>>>
        unexplored_solutions;
    std::vector<double> costs(adj.size(), 0.0);
    unsigned num_objectives = adj.size(),
             num_vertices = adj.front().size(),
             num_improvments = 0;
    std::pair<std::vector<unsigned>, std::vector<double>> solution;
    std::vector<unsigned> curr_cycle, new_cycle;
    std::vector<double> curr_costs, new_costs;
    std::vector<std::pair<std::vector<unsigned>, std::vector<double>>>
        new_solutions;

    for(unsigned i = 0; i < num_vertices; i++) {
        unsigned u = cycle[i],
                 v = cycle[(i + 1) % num_vertices];
        for(unsigned j = 0; j < num_objectives; j++) {
            costs[j] += adj[j][u][v];
        }
    }

    unexplored_solutions.push_back(std::make_pair(cycle, costs));

    while(!unexplored_solutions.empty() &&
          num_improvments < max_num_improvements) {
        solution = unexplored_solutions.front();
        unexplored_solutions.pop_front();

        curr_cycle = solution.first;
        curr_costs = solution.second;

        bool is_dominated_or_equal = false;

        for(unsigned i = 0; i < num_vertices - 1; i++) {
            unsigned u1 = curr_cycle[i],
                     v1 = curr_cycle[(i + 1) % num_vertices];

            for(unsigned j = i + 2; j < num_vertices; j++) {
                unsigned u2 = curr_cycle[j],
                         v2 = curr_cycle[(j + 1) % num_vertices];
                new_cycle = curr_cycle;
                new_costs = curr_costs;

                reverse(new_cycle.begin() + i + 1, new_cycle.begin() + j + 1);

                std::vector<unsigned>::iterator it = find(new_cycle.begin(),
                                                          new_cycle.end(),
                                                          0);
                if(it != new_cycle.end()) {
                    std::rotate(new_cycle.begin(), it, new_cycle.end());
                }

                if(new_cycle[1] > new_cycle.back()) {
                    std::reverse(new_cycle.begin() + 1, new_cycle.end());
                }

                for(unsigned l = 0; l < num_objectives; l++) {
                    new_costs[l] -= adj[l][u1][v1] + adj[l][u2][v2];
                    new_costs[l] += adj[l][u1][u2] + adj[l][v1][v2];
                }

                if(Solution::dominates(new_costs, curr_costs) ||
                   std::equal(new_costs.begin(),
                              new_costs.end(),
                              curr_costs.begin(),
                              [](double a, double b) {
                                  return fabs(a - b) <
                                        std::numeric_limits<double>::epsilon();
                              })) {
                    is_dominated_or_equal = false;

                    for(auto it = new_solutions.begin();
                             it != new_solutions.end();) {
                        auto solution = *it;

                        if(Solution::dominates(new_costs, solution.second)) {
                            it = new_solutions.erase(it);
                        } else {
                            if(Solution::dominates(solution.second, new_costs)
                               || std::equal(solution.second.begin(),
                                             solution.second.end(),
                                             new_costs.begin(),
                                             [](double a, double b) {
                                                 return fabs(a - b) <
                                                    std::numeric_limits<double>::epsilon();
                                             })) {
                                is_dominated_or_equal = true;
                                break;
                            }

                            it++;
                        }
                    }

                    if(!is_dominated_or_equal &&
                       new_solutions.size() < max_num_solutions) {
                        new_solutions.push_back(std::make_pair(new_cycle,
                                                               new_costs));
                    }
                }
            }
        }

        for(auto it1 = new_solutions.begin(); it1 != new_solutions.end();) {
            auto newSolution = *it1;
            is_dominated_or_equal = false;

            for(auto it2 = non_dominated_solutions.begin();
                     it2 != non_dominated_solutions.end();) {
                auto solution = *it2;

                if(Solution::dominates(newSolution.second, solution.second)) {
                    it2 = non_dominated_solutions.erase(it2);
                } else {
                    if(Solution::dominates(solution.second, newSolution.second)
                       || std::equal(solution.second.begin(),
                                     solution.second.end(),
                                     newSolution.second.begin(),
                                     [](double a, double b) {
                                         return fabs(a - b) <
                                            std::numeric_limits<double>::epsilon();
                                     })) {
                        is_dominated_or_equal = true;
                        break;
                    }

                    it2++;
                }
            }

            if(is_dominated_or_equal) {
                it1 = new_solutions.erase(it1);
            } else {
                if(non_dominated_solutions.size() < max_num_solutions) {
                    non_dominated_solutions.push_back(newSolution);
                }

                it1++;
            }
        }

        for(auto newSolution : new_solutions) {
            is_dominated_or_equal = false;

            for(auto it = unexplored_solutions.begin();
                     it != unexplored_solutions.end();) {
                auto solution = *it;
                if(Solution::dominates(newSolution.second, solution.second)) {
                    it = unexplored_solutions.erase(it);
                } else {
                    if(Solution::dominates(solution.second, newSolution.second)
                       || std::equal(solution.second.begin(),
                                     solution.second.end(),
                                     newSolution.second.begin(),
                                     [](double a, double b) {
                                        return fabs(a - b) <
                                            std::numeric_limits<double>::epsilon();
                                     })) {
                        is_dominated_or_equal = true;
                        break;
                    }

                    it++;
                }
            }

            if(!is_dominated_or_equal &&
               unexplored_solutions.size() < max_num_solutions) {
                unexplored_solutions.push_back(newSolution);
            }
        }

        num_improvments++;
    }

    std::vector<std::vector<unsigned>> result(non_dominated_solutions.size());

    std::transform(non_dominated_solutions.begin(),
                   non_dominated_solutions.end(),
                   result.begin(),
                   [](const auto & solution) {
                        return solution.first;
                   });

    return result;
}

}

