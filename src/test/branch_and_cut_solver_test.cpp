#include "solver/weighted_sum/branch_and_cut/branch_and_cut_solver.hpp"
#include <cassert>

int main() {
    std::ifstream ifs;
    motsp::Instance instance;
    motsp::Branch_and_Cut_Solver solver;

    std::cout << "instances/kroAC100.txt" << std::endl;

    ifs.open("instances/kroAC100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::Branch_and_Cut_Solver(instance);

    solver.time_limit = 5.0;
    solver.iterations_limit = 32;
    solver.max_num_solutions = 10;
    solver.max_num_snapshots = 5;

    assert(fabs(solver.time_limit - 5.0) <
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

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost.size() == instance.num_objectives);
        assert(s1.cost[0] >= 21282);
        assert(s1.cost[1] >= 20749);
        for(const auto & s2 : solver.best_solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    for(const auto & snapshot : solver.best_solutions_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & s : std::get<2>(snapshot)) {
            assert(s.size() == instance.num_objectives);
            assert(s[0] >= 21282);
            assert(s[1] >= 20749);
        }
    }

    for(const auto & snapshot : solver.num_non_dominated_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_non_dominated : std::get<2>(snapshot)) {
            assert(num_non_dominated > 0);
            assert(num_non_dominated <= solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.num_fronts_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_fronts : std::get<2>(snapshot)) {
            assert(num_fronts > 0);
            assert(num_fronts < solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.populations_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & population : std::get<2>(snapshot)) {
            assert(population.size() == solver.max_num_solutions);

            for(const auto & s : population) {
                assert(s.size() == instance.num_objectives);
                assert(s[0] >= 21282);
                assert(s[1] >= 20749);
            }
        }
    }

    std::cout << solver << std::endl;

    std::cout << "Num non dominated snapshots: ";
    for(unsigned i = 0;
        i < solver.num_non_dominated_snapshots.size() - 1;
        i++) {
        std::cout << std::get<2>(solver.num_non_dominated_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_non_dominated_snapshots.back()).front()
              << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << std::get<2>(solver.num_fronts_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_fronts_snapshots.back()).front()
              << std::endl << std::endl;

    std::cout << "instances/kroAD100.txt" << std::endl;

    ifs.open("instances/kroAC100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::Branch_and_Cut_Solver(instance);

    solver.time_limit = 5.0;
    solver.iterations_limit = 32;
    solver.max_num_solutions = 10;
    solver.max_num_snapshots = 5;

    assert(fabs(solver.time_limit - 5.0) <
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

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost.size() == instance.num_objectives);
        assert(s1.cost[0] >= 21282);
        assert(s1.cost[1] >= 20749);
        for(const auto & s2 : solver.best_solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    for(const auto & snapshot : solver.best_solutions_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & s : std::get<2>(snapshot)) {
            assert(s.size() == instance.num_objectives);
            assert(s[0] >= 21282);
            assert(s[1] >= 20749);
        }
    }

    for(const auto & snapshot : solver.num_non_dominated_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_non_dominated : std::get<2>(snapshot)) {
            assert(num_non_dominated > 0);
            assert(num_non_dominated <= solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.num_fronts_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_fronts : std::get<2>(snapshot)) {
            assert(num_fronts > 0);
            assert(num_fronts < solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.populations_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & population : std::get<2>(snapshot)) {
            assert(population.size() == solver.max_num_solutions);

            for(const auto & s : population) {
                assert(s.size() == instance.num_objectives);
                assert(s[0] >= 21282);
                assert(s[1] >= 20749);
            }
        }
    }

    std::cout << solver << std::endl;

    std::cout << "Num non dominated snapshots: ";
    for(unsigned i = 0;
        i < solver.num_non_dominated_snapshots.size() - 1;
        i++) {
        std::cout << std::get<2>(solver.num_non_dominated_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_non_dominated_snapshots.back()).front()
              << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << std::get<2>(solver.num_fronts_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_fronts_snapshots.back()).front()
              << std::endl << std::endl;

    std::cout << "instances/kroACD100.txt" << std::endl;

    ifs.open("instances/kroCD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::Branch_and_Cut_Solver(instance);

    solver.time_limit = 5.0;
    solver.iterations_limit = 32;
    solver.max_num_solutions = 10;
    solver.max_num_snapshots = 5;

    assert(fabs(solver.time_limit - 5.0) <
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

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost.size() == instance.num_objectives);
        assert(s1.cost[0] >= 20749);
        assert(s1.cost[1] >= 21294);
        for(const auto & s2 : solver.best_solutions) {
            assert(!s1.dominates(s2));
            assert(!s2.dominates(s1));
        }
    }

    for(const auto & snapshot : solver.best_solutions_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & s : std::get<2>(snapshot)) {
            assert(s.size() == instance.num_objectives);
            assert(s[0] >= 20749);
            assert(s[1] >= 21294);
        }
    }

    for(const auto & snapshot : solver.num_non_dominated_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_non_dominated : std::get<2>(snapshot)) {
            assert(num_non_dominated > 0);
            assert(num_non_dominated <= solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.num_fronts_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_fronts : std::get<2>(snapshot)) {
            assert(num_fronts > 0);
            assert(num_fronts < solver.max_num_solutions);
        }
    }

    for(const auto & snapshot : solver.populations_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const auto & population : std::get<2>(snapshot)) {
            assert(population.size() == solver.max_num_solutions);

            for(const auto & s : population) {
                assert(s.size() == instance.num_objectives);
                assert(s[0] >= 20749);
                assert(s[1] >= 21294);
            }
        }
    }

    std::cout << solver << std::endl;

    std::cout << "Num non dominated snapshots: ";
    for(unsigned i = 0;
        i < solver.num_non_dominated_snapshots.size() - 1;
        i++) {
        std::cout << std::get<2>(solver.num_non_dominated_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_non_dominated_snapshots.back()).front()
              << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << std::get<2>(solver.num_fronts_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_fronts_snapshots.back()).front()
              << std::endl << std::endl;

    std::cout << "Branch-and-Cut Solver Test PASSED" << std::endl;

    return 0;
}
