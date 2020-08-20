#include "solver/nspso/nspso_solver.hpp"

int main() {
    std::ifstream ifs;
    motsp::Instance instance;

    ifs.open("instances/kroACD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    motsp::NSPSO_Solver solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 10.0;
    solver.iterations_limit = 200;
    solver.max_num_solutions = 128;
    solver.population_size = 32;
    solver.max_num_snapshots = 32;

    assert(solver.seed = 2351389233);
    assert(fabs(solver.time_limit - 10.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 200);
    assert(solver.max_num_solutions == 128);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 32);
    assert(fabs(solver.omega - 0.6) < std::numeric_limits<double>::epsilon());
    assert(fabs(solver.c1 - 2.0) < std::numeric_limits<double>::epsilon());
    assert(fabs(solver.c2 - 2.0) < std::numeric_limits<double>::epsilon());
    assert(fabs(solver.chi - 1.0) < std::numeric_limits<double>::epsilon());
    assert(fabs(solver.v_coeff - 0.5) < std::numeric_limits<double>::epsilon());
    assert(solver.leader_selection_range == 60);
    assert(solver.diversity_mechanism == "crowding distance");
    assert(solver.memory);

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots > 0);
    assert(solver.num_snapshots <= solver.max_num_snapshots + 1);

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

    for(const auto & pareto_snapshot : solver.pareto_snapshots) {
        assert(std::get<0>(pareto_snapshot) >= 0);
        assert(std::get<0>(pareto_snapshot) <= solver.num_iterations);
        assert(std::get<1>(pareto_snapshot) >= 0.0);
        assert(std::get<1>(pareto_snapshot) <= solver.solving_time);
        assert(std::get<2>(pareto_snapshot).size() > 0);
        assert(std::get<2>(pareto_snapshot).size() <= solver.max_num_solutions);

        for(const auto & s : std::get<2>(pareto_snapshot)) {
            assert(s.size() == instance.num_objectives);
        }
    }

    std::cout << solver << std::endl;

    std::cout << "NSPSO Solver Test PASSED" << std::endl;

    return 0;
}

