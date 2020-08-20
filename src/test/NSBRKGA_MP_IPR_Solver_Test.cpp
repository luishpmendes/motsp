#include "../solver/nsbrkga_mp_ipr/NSBRKGA_MP_IPR_Solver.hpp"
#include <cassert>

int main() {
    MOTSP::Instance instance;
    MOTSP::NSBRKGA_MP_IPR_Solver solver;
    std::vector<MOTSP::Solution> solutions;

    instance = MOTSP::Instance("instances/kroACD100.txt");
    solver = MOTSP::NSBRKGA_MP_IPR_Solver(instance);

    solver.setSeed(0);
    solver.setTimeLimit(10.0);
    solver.setPopulationSize(32);

    assert(fabs(solver.getTimeLimit() - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getPopulationSize() == 32);
    assert(fabs(solver.getElitePercentage() - 0.17) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getMutantPercentage() - 0.19) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getNumTotalParents() == 5);
    assert(solver.getNumEliteParents() == 3);
    assert(solver.getBiasType() == BRKGA::BiasFunctionType::LOGINVERSE);
    assert(solver.getNumPopulations() == 1);
    assert(fabs(solver.getPrPercentagePairs() - 0.75) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.getPrMinDist() - 0.15) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getPrSelection() ==
            BRKGA::PathRelinking::Selection::BESTSOLUTION);
    assert(fabs(solver.getPrPercentage() - 0.31) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getPrInterval() == 100);
    assert(solver.getShakeInterval() == 200);
    assert(solver.getResetInterval() == 500);
    assert(solver.getNumThreads() == 1);

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

    std::cout << "NSBRGKA MP IPR Solver Test PASSED" << std::endl;

    return 0;
}

