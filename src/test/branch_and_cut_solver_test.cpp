#include "solver/weighted_sum/branch_and_cut/branch_and_cut_solver.hpp"
#include <cassert>

int main() {
    std::ifstream ifs;
    motsp::Instance instance;

    ifs.open("instances/kroACD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    motsp::Branch_and_Cut_Solver solver(instance);

    solver.time_limit = 10.0;
    solver.iterations_limit = 32;
    solver.max_num_solutions = 10;
    solver.max_num_snapshots = 5;

    assert(fabs(solver.time_limit - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 32);
    assert(solver.max_num_solutions == 10);
    assert(solver.max_num_snapshots == 5);

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots > 0);
    assert(solver.num_snapshots <= solver.max_num_snapshots + 2);

    assert(solver.pareto_snapshots.size() == solver.num_snapshots);
    assert(solver.non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.fronts_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost[0] >= 21282);
        assert(s1.cost[1] >= 20749);
        assert(s1.cost[2] >= 21294);
        for(const auto & s2 : solver.best_solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    std::cout << solver << std::endl;

    std::cout << "Branch-and-Cut Solver Test PASSED" << std::endl;

    return 0;
}

