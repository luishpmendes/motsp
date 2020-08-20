#include "../solution/Solution.hpp"
#include "../solver/local_search/TwoOpt.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

int main() {
    MOTSP::Instance instance;
    std::vector<unsigned> cycle;
    std::vector<std::vector<unsigned>> improvedCycles;
    MOTSP::Solution solution;
    std::vector<MOTSP::Solution> improvedSolutions;

    instance = MOTSP::Instance("instances/kroACD100.txt");

    cycle = std::vector<unsigned>{ 0, 46, 92, 27, 66, 57, 60, 50, 86, 24, 
                                  80, 68, 63, 39, 53,  1, 43, 49, 72, 67, 
                                  84, 81, 94, 12, 75, 32, 36,  4, 51, 77, 
                                  95, 38, 29, 47, 99, 40, 70, 13,  2, 42,
                                  45, 28, 33, 82, 54,  6,  8, 56, 19, 11, 
                                  26, 85, 34, 61, 59, 76, 22, 97, 90, 44, 
                                  31, 10, 14, 16, 58, 73, 20, 71,  9, 83, 
                                  35, 98, 37, 23, 17, 78, 52, 87, 15, 93, 
                                  21, 69, 65, 25, 64,  3, 96, 55, 79, 30, 
                                  88, 41,  7, 91, 74, 18, 89, 48,  5, 62};
    solution = MOTSP::Solution(instance, cycle);
    assert(solution.isFeasible(instance));
    assert(fabs(solution.getCost()[0] - 21282) <
            std::numeric_limits<double>::epsilon());
    assert(solution.getCost()[1] >= 20749);
    assert(solution.getCost()[2] >= 21294);
    improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(), cycle);
    improvedSolutions.resize(improvedCycles.size());
    std::transform(improvedCycles.begin(),
                   improvedCycles.end(),
                   improvedSolutions.begin(),
                   [&](const std::vector<unsigned> & c) {
                        return MOTSP::Solution(instance, c);
                   });
    for(const MOTSP::Solution & improvedSolution : improvedSolutions) {
        assert(improvedSolution.isFeasible(instance));
        assert(fabs(improvedSolution.getCost()[0] - 21282) < 
                std::numeric_limits<double>::epsilon());
        assert(improvedSolution.getCost()[1] >= 20749);
        assert(improvedSolution.getCost()[2] >= 21294);
        assert(improvedSolution.getCost()[1] <= solution.getCost()[1]);
        assert(improvedSolution.getCost()[2] <= solution.getCost()[2]);
        assert(!solution.dominates(improvedSolutions.front()));
        for(const MOTSP::Solution & otherImprovedSolution : improvedSolutions) {
            assert(!improvedSolution.dominates(otherImprovedSolution));
            assert(!otherImprovedSolution.dominates(improvedSolution));
        }
        improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(),
                                                improvedSolution.getCycle());
        assert(improvedCycles.size() == 1);
        MOTSP::Solution newImprovedSolution(instance, improvedCycles.front());
        for(unsigned i = 0; i < instance.getNumObjectives(); i++) {
            assert(fabs(newImprovedSolution.getCost()[i] 
                        - improvedSolution.getCost()[i]) <
                    std::numeric_limits<double>::epsilon());
        }
        for(unsigned i = 0; i < instance.getNumVertices(); i++) {
            assert(newImprovedSolution.getCycle()[i] ==
                    improvedSolution.getCycle()[i]);
        }
    }

    cycle = std::vector<unsigned>{ 0, 52, 39, 11, 48, 17, 28, 45, 23, 31,
                                  60, 25,  6, 81, 77,  8, 36, 15, 50, 62,
                                  43, 65, 47, 83, 10, 51, 86, 95, 96, 80,
                                  44, 32, 99, 73, 56, 35, 13,  9, 91, 18,
                                  98, 92,  3, 59, 68,  2, 72, 58, 40, 88,
                                  20, 22, 69, 75, 90, 93, 94, 49, 61, 82,
                                  71, 85,  4, 42, 55, 70, 37, 38, 27, 87,
                                  97, 57, 33, 89, 24, 16,  7, 21, 74,  5,
                                  53,  1, 34, 67, 29, 76, 79, 64, 30, 46,
                                  66, 54, 41, 19, 63, 78, 12, 14, 26, 84};
    solution = MOTSP::Solution(instance, cycle);
    assert(solution.isFeasible(instance));
    assert(solution.getCost()[0] >= 21282);
    assert(fabs(solution.getCost()[1] - 20749) <
            std::numeric_limits<double>::epsilon());
    assert(solution.getCost()[2] >= 21294);
    improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(), cycle);
    improvedSolutions.resize(improvedCycles.size());
    std::transform(improvedCycles.begin(),
                   improvedCycles.end(),
                   improvedSolutions.begin(),
                   [&](const std::vector<unsigned> & c) {
                        return MOTSP::Solution(instance, c);
                   });
    for(const MOTSP::Solution & improvedSolution : improvedSolutions) {
        assert(improvedSolution.isFeasible(instance));
        assert(improvedSolution.getCost()[0] >= 21282);
        assert(fabs(improvedSolution.getCost()[1] - 20749) <
                std::numeric_limits<double>::epsilon());
        assert(improvedSolution.getCost()[2] >= 21294);
        assert(improvedSolution.getCost()[0] <= solution.getCost()[0]);
        assert(improvedSolution.getCost()[2] <= solution.getCost()[0]);
        assert(!solution.dominates(improvedSolutions.front()));
        for(const MOTSP::Solution & otherImprovedSolution : improvedSolutions) {
            assert(!improvedSolution.dominates(otherImprovedSolution));
            assert(!otherImprovedSolution.dominates(improvedSolution));
        }
        improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(),
                                                improvedSolution.getCycle());
        assert(improvedCycles.size() == 1);
        MOTSP::Solution newImprovedSolution(instance, improvedCycles.front());
        for(unsigned i = 0; i < instance.getNumObjectives(); i++) {
            assert(fabs(newImprovedSolution.getCost()[i] 
                        - improvedSolution.getCost()[i]) <
                    std::numeric_limits<double>::epsilon());
        }
        for(unsigned i = 0; i < instance.getNumVertices(); i++) {
            assert(newImprovedSolution.getCycle()[i] ==
                    improvedSolution.getCycle()[i]);
        }
    }

    cycle = std::vector<unsigned>{ 0, 12, 78, 87, 26, 23, 40, 83, 68, 14,
                                  72, 38, 70,  3, 92, 43, 59, 52,  4, 61,
                                  36, 18, 50, 34, 29, 99,  1, 47, 91, 54,
                                  86, 25, 94, 55, 27, 11, 46, 20,  9,  8,
                                  19, 24, 81, 17,  5, 39, 82,  2, 58, 84,
                                  63, 42, 76, 90, 53, 62, 15, 95, 85, 64,
                                  13, 67, 32, 44, 98, 77, 30, 56, 71, 79,
                                  75, 60, 73,  6, 74, 66, 35, 28, 57, 21,
                                  69, 48, 97, 96, 31, 93, 16, 89, 10, 88,
                                  41, 22, 45, 51,  7, 65, 37, 80, 33, 49};
    solution = MOTSP::Solution(instance, cycle);
    assert(solution.isFeasible(instance));
    assert(solution.getCost()[0] >= 21282);
    assert(solution.getCost()[1] >= 20749);
    assert(fabs(solution.getCost()[2] - 21294) <
            std::numeric_limits<double>::epsilon());
    improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(), cycle);
    improvedSolutions.resize(improvedCycles.size());
    std::transform(improvedCycles.begin(),
                   improvedCycles.end(),
                   improvedSolutions.begin(),
                   [&](const std::vector<unsigned> & c) {
                        return MOTSP::Solution(instance, c);
                   });
    for(const MOTSP::Solution & improvedSolution : improvedSolutions) {
        assert(improvedSolution.isFeasible(instance));
        assert(improvedSolution.getCost()[0] >= 21282);
        assert(improvedSolution.getCost()[1] >= 20749);
        assert(fabs(improvedSolution.getCost()[2] - 21294) <
                std::numeric_limits<double>::epsilon());
        assert(improvedSolution.getCost()[0] <= solution.getCost()[0]);
        assert(improvedSolution.getCost()[1] <= solution.getCost()[1]);
        assert(!solution.dominates(improvedSolutions.front()));
        for(const MOTSP::Solution & otherImprovedSolution : improvedSolutions) {
            assert(!improvedSolution.dominates(otherImprovedSolution));
            assert(!otherImprovedSolution.dominates(improvedSolution));
        }
        improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(),
                                                improvedSolution.getCycle());
        assert(improvedCycles.size() == 1);
        MOTSP::Solution newImprovedSolution(instance, improvedCycles.front());
        for(unsigned i = 0; i < instance.getNumObjectives(); i++) {
            assert(fabs(newImprovedSolution.getCost()[i] 
                        - improvedSolution.getCost()[i]) <
                    std::numeric_limits<double>::epsilon());
        }
        for(unsigned i = 0; i < instance.getNumVertices(); i++) {
            assert(newImprovedSolution.getCycle()[i] ==
                    improvedSolution.getCycle()[i]);
        }
    }

    shuffle(cycle.begin(), cycle.end(), std::mt19937(1248));
    solution = MOTSP::Solution(instance, cycle);
    assert(solution.isFeasible(instance));
    assert(solution.getCost()[0] >= 21282);
    assert(solution.getCost()[1] >= 20749);
    assert(solution.getCost()[2] >= 21294);
    improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(), cycle);
    improvedSolutions.resize(improvedCycles.size());
    std::transform(improvedCycles.begin(),
                   improvedCycles.end(),
                   improvedSolutions.begin(),
                   [&](const std::vector<unsigned> & c) {
                        return MOTSP::Solution(instance, c);
                   });
    for(const MOTSP::Solution & improvedSolution : improvedSolutions) {
        assert(improvedSolution.isFeasible(instance));
        assert(improvedSolution.getCost()[0] >= 21282);
        assert(improvedSolution.getCost()[1] >= 20749);
        assert(improvedSolution.getCost()[2] >= 21294);
        assert(improvedSolution.getCost()[0] <= solution.getCost()[0]);
        assert(improvedSolution.getCost()[1] <= solution.getCost()[1]);
        assert(!solution.dominates(improvedSolutions.front()));
        for(const MOTSP::Solution & otherImprovedSolution : improvedSolutions) {
            assert(!improvedSolution.dominates(otherImprovedSolution));
            assert(!otherImprovedSolution.dominates(improvedSolution));
        }
        improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(),
                                                improvedSolution.getCycle());
        assert(improvedCycles.size() == 1);
        MOTSP::Solution newImprovedSolution(instance, improvedCycles.front());
        for(unsigned i = 0; i < instance.getNumObjectives(); i++) {
            assert(fabs(newImprovedSolution.getCost()[i] 
                        - improvedSolution.getCost()[i]) <
                    std::numeric_limits<double>::epsilon());
        }
        for(unsigned i = 0; i < instance.getNumVertices(); i++) {
            assert(newImprovedSolution.getCycle()[i] ==
                    improvedSolution.getCycle()[i]);
        }
    }

    std::vector<std::vector<std::vector<double>>> adj(1,
            std::vector<std::vector<double>>(4, std::vector<double>(4, 0.0)));
    adj[0][0][1] = 1;
    adj[0][1][0] = 1;
    adj[0][1][3] = 1;
    adj[0][3][1] = 1;
    adj[0][2][3] = 1;
    adj[0][3][2] = 1;
    adj[0][2][0] = 1;
    adj[0][0][2] = 1;
    adj[0][0][3] = 2;
    adj[0][3][0] = 2;
    adj[0][1][2] = 2;
    adj[0][2][1] = 2;
    instance = MOTSP::Instance(adj);
    cycle = std::vector<unsigned>(4, 0);
    cycle[0] = 0;
    cycle[1] = 1;
    cycle[2] = 2;
    cycle[3] = 3;
    solution = MOTSP::Solution(instance, cycle);
    improvedCycles = MOTSP::TwoOpt::improve(instance.getAdj(), cycle);
    assert(improvedCycles.size() == 1);
    MOTSP::Solution improvedSolution(instance, improvedCycles[0]);
    assert(improvedSolution.dominates(solution));

    std::cout << "TwoOpt Test PASSED" << std::endl;

    return 0;
}

