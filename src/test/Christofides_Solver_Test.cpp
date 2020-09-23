#include "../solver/weighted_sum/christofides/Christofides_Solver.hpp"
#include <cassert>

int main() {
    MOTSP::Instance instance;
    MOTSP::Christofides_Solver solver;
    std::vector<MOTSP::Solution> solutions;

    instance = MOTSP::Instance("instances/kroACD100.txt");
    solver = MOTSP::Christofides_Solver(instance);

    solver.setTimeLimit(10.0);
    solver.setMaxNumSolutions(100);

    assert(fabs(solver.getTimeLimit() - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.getMaxNumSolutions() == 100);

    solver.solve();

    assert(solver.getSolvingTime() > 0);

    solutions = solver.getSolutions();

    assert(solutions.size() > 0);
    assert(solver.getMaxNumSolutions() == 0 || 
           solutions.size() <= solver.getMaxNumSolutions());

    for(unsigned i = 0; i < solutions.size(); i++) {
        assert(solutions[i].isFeasible(instance));
        assert(solutions[i].getCost()[0] >= 21282);
        assert(solutions[i].getCost()[1] >= 20749);
        assert(solutions[i].getCost()[2] >= 21294);

        for(unsigned j = 0; j < solutions.size(); j++) {
            assert(!solutions[i].dominates(solutions[j]));
            assert(!solutions[j].dominates(solutions[i]));
        }
    }

    std::cout << "Christofides Solver Test PASSED" << std::endl;

    return 0;
}

