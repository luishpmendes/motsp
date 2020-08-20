#include "../solver/nsga2/NSGA2_Solver.hpp"

int main() {
    MOTSP::Instance instance;
    MOTSP::NSGA2_Solver solver;
    std::vector<MOTSP::Solution> solutions;

    instance = MOTSP::Instance("instances/kroACD100.txt");
    solver = MOTSP::NSGA2_Solver(instance);

    solver.setTimeLimit(10.0);
    solver.setPopulationSize(32);

    assert(fabs(solver.getTimeLimit() - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getPopulationSize() == 32);
    assert(fabs(solver.getCrossoverProbability() - 0.95) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getCrossoverDistribution() - 10.00) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getMutationProbability() - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getMutationDistribution() - 50.00) <
            std::numeric_limits<double>::epsilon());

    solver.solve();

    assert(solver.getSolvingTime() > 0);
    assert(solver.getNumGenerations() > 0);

    solutions = solver.getSolutions();

    assert(solutions.size() > 0);
    assert(solutions.size() <= solver.getMaxNumSolutions());

    for(const MOTSP::Solution & s1 : solutions) {
        assert(s1.isFeasible(instance));
        assert(s1.getCost()[0] >= 21282);
        assert(s1.getCost()[1] >= 20749);
        assert(s1.getCost()[2] >= 21294);

        for(const MOTSP::Solution & s2 : solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    std::cout << "NSGA2 Solver Test PASSED" << std::endl;

    return 0;
}

