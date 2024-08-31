#include "solver/nsga2/nsga2_solver.hpp"

int main() {
    std::ifstream ifs;
    motsp::Instance instance;
    motsp::NSGA2_Solver solver;

    std::cout << "instances/kroAC100.txt" << std::endl;

    ifs.open("instances/kroAC100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSGA2_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 100;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 100);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.crossover_probability - 0.95) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.crossover_distribution - 10.00) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.00) <
            std::numeric_limits<double>::epsilon());

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots == solver.max_num_snapshots);

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
    assert(solver.populations_snapshots.size() == solver.num_snapshots);

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
            assert(num_non_dominated <= solver.population_size);
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
            assert(num_fronts < solver.population_size);
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
            assert(population.size() == solver.population_size);

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
        std::cout << "(" 
                  << std::get<0>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_non_dominated_snapshots[i]).begin(),
            std::get<2>(solver.num_non_dominated_snapshots[i]).end(),
            0) / std::get<2>(solver.num_non_dominated_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::get<1>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_non_dominated_snapshots.back()).begin(),
        std::get<2>(solver.num_non_dominated_snapshots.back()).end(),
        0) / std::get<2>(solver.num_non_dominated_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_fronts_snapshots[i]).begin(),
            std::get<2>(solver.num_fronts_snapshots[i]).end(),
            0) / std::get<2>(solver.num_fronts_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_fronts_snapshots.back())
              << ", "
              << std::get<1>(solver.num_fronts_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_fronts_snapshots.back()).begin(),
        std::get<2>(solver.num_fronts_snapshots.back()).end(),
        0) / std::get<2>(solver.num_fronts_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "instances/kroAD100.txt" << std::endl;

    ifs.open("instances/kroAD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSGA2_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 100;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 100);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.crossover_probability - 0.95) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.crossover_distribution - 10.00) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.00) <
            std::numeric_limits<double>::epsilon());

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots == solver.max_num_snapshots);

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
    assert(solver.populations_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost.size() == instance.num_objectives);
        assert(s1.cost[0] >= 21282);
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
            assert(s[0] >= 21282);
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
            assert(num_non_dominated <= solver.population_size);
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
            assert(num_fronts < solver.population_size);
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
            assert(population.size() == solver.population_size);

            for(const auto & s : population) {
                assert(s.size() == instance.num_objectives);
                assert(s[0] >= 21282);
                assert(s[1] >= 21294);
            }
        }
    }

    std::cout << solver << std::endl;

    std::cout << "Num non dominated snapshots: ";
    for(unsigned i = 0;
        i < solver.num_non_dominated_snapshots.size() - 1;
        i++) {
        std::cout << "(" 
                  << std::get<0>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_non_dominated_snapshots[i]).begin(),
            std::get<2>(solver.num_non_dominated_snapshots[i]).end(),
            0) / std::get<2>(solver.num_non_dominated_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::get<1>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_non_dominated_snapshots.back()).begin(),
        std::get<2>(solver.num_non_dominated_snapshots.back()).end(),
        0) / std::get<2>(solver.num_non_dominated_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_fronts_snapshots[i]).begin(),
            std::get<2>(solver.num_fronts_snapshots[i]).end(),
            0) / std::get<2>(solver.num_fronts_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_fronts_snapshots.back())
              << ", "
              << std::get<1>(solver.num_fronts_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_fronts_snapshots.back()).begin(),
        std::get<2>(solver.num_fronts_snapshots.back()).end(),
        0) / std::get<2>(solver.num_fronts_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "instances/kroCD100.txt" << std::endl;

    ifs.open("instances/kroCD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSGA2_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 100;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 100);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.crossover_probability - 0.95) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.crossover_distribution - 10.00) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.00) <
            std::numeric_limits<double>::epsilon());

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots == solver.max_num_snapshots);

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
    assert(solver.populations_snapshots.size() == solver.num_snapshots);

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
            assert(num_non_dominated <= solver.population_size);
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
            assert(num_fronts < solver.population_size);
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
            assert(population.size() == solver.population_size);

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
        std::cout << "(" 
                  << std::get<0>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_non_dominated_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_non_dominated_snapshots[i]).begin(),
            std::get<2>(solver.num_non_dominated_snapshots[i]).end(),
            0) / std::get<2>(solver.num_non_dominated_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::get<1>(solver.num_non_dominated_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_non_dominated_snapshots.back()).begin(),
        std::get<2>(solver.num_non_dominated_snapshots.back()).end(),
        0) / std::get<2>(solver.num_non_dominated_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "Num fronts snapshots: ";
    for(unsigned i = 0; i < solver.num_fronts_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_fronts_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_fronts_snapshots[i]).begin(),
            std::get<2>(solver.num_fronts_snapshots[i]).end(),
            0) / std::get<2>(solver.num_fronts_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_fronts_snapshots.back())
              << ", "
              << std::get<1>(solver.num_fronts_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_fronts_snapshots.back()).begin(),
        std::get<2>(solver.num_fronts_snapshots.back()).end(),
        0) / std::get<2>(solver.num_fronts_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << std::endl << "NSGA2 Solver Test PASSED" << std::endl;

    return 0;
}
