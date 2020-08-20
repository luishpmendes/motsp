#include "../solver/nspso/NSPSO_Solver.hpp"

int main() {
    MOTSP::Instance instance;
    MOTSP::NSPSO_Solver solver;
    std::vector<MOTSP::Solution> solutions;

    instance = MOTSP::Instance("instances/kroACD100.txt");
    solver = MOTSP::NSPSO_Solver(instance);

    solver.setTimeLimit(10.0);
    solver.setPopulationSize(32);

    assert(fabs(solver.getTimeLimit() - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getPopulationSize() == 32);
    assert(fabs(solver.getOmega() - 0.6) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getC1() - 2.0) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getC2() - 2.0) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getChi() - 1.0) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getVCoeff() - 0.5) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getLeaderSelectionRange() == 60);
    assert(solver.getDiversityMechanism() == "crowding distance");
    assert(solver.getMemory());

    solver.solve();

    assert(solver.getSolvingTime() > 0);
    assert(solver.getNumGenerations() > 0);

    solutions = solver.getSolutions();

    assert(solutions.size() > 0);
    assert(solutions.size() <= solver.getMaxNumSolutions());

    for(const auto & s1 : solutions) {
        assert(s1.isFeasible(instance));
        assert(s1.getCost()[0] >= 21282);
        assert(s1.getCost()[1] >= 20749);
        assert(s1.getCost()[2] >= 21294);

        for(const auto & s2 : solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    std::cout << "NSPSO Solver Test PASSED" << std::endl;

    return 0;
}

