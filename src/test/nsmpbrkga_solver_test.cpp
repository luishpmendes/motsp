#include "solver/nsmpbrkga/nsmpbrkga_solver.hpp"
#include <cassert>

int main() {
    std::ifstream ifs;
    motsp::Instance instance;

    ifs.open("instances/kroACD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    motsp::NSMPBRKGA_Solver solver(instance);

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
    assert(fabs(solver.min_elites_percentage - 0.10) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.max_elites_percentage - 0.50) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.min_mutants_percentage - 0.05) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.max_mutants_percentage - 0.25) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_total_parents == 3);
    assert(solver.num_elite_parents == 2);
    assert(solver.bias_type == BRKGA::BiasFunctionType::LOGINVERSE);
    assert(solver.diversity_type ==
            BRKGA::DiversityFunctionType::AVERAGE_DISTANCE_TO_CENTROID);
    assert(solver.num_populations == 1);
    assert(solver.pr_number_pairs = 100);
    assert(fabs(solver.pr_min_dist - 0.15) <
            std::numeric_limits<double>::epsilon());
    assert(solver.pr_selection ==
            BRKGA::PathRelinking::Selection::BESTSOLUTION);
    assert(fabs(solver.pr_percentage - 0.31) <
            std::numeric_limits<double>::epsilon());
    assert(solver.pr_interval == 0);
    assert(solver.shake_interval == 200);
    assert(solver.reset_interval == 500);
    assert(solver.num_threads == 1);

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots > 0);
    assert(solver.num_snapshots <= solver.max_num_snapshots + 1);

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
    assert(solver.populations_snapshots.size() == solver.num_snapshots);
    assert(solver.num_elites_snapshots.size() == solver.num_snapshots);
    assert(solver.num_mutants_snapshots.size() == solver.num_snapshots);

    for(const auto & s1 : solver.best_solutions) {
        assert(s1.is_feasible());
        assert(s1.cost.size() == instance.num_objectives);
        assert(s1.cost[0] >= 21282);
        assert(s1.cost[1] >= 20749);
        assert(s1.cost[2] >= 21294);

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
            assert(s[2] >= 21294);
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
            assert(num_fronts <= solver.population_size);
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
                assert(s[2] >= 21294);
            }
        }
    }

    for(const auto & snapshot : solver.num_elites_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_elites : std::get<2>(snapshot)) {
            assert(num_elites >= floor(solver.population_size
                                     * solver.min_elites_percentage));
            assert(num_elites <= ceil(solver.population_size
                                    * solver.max_elites_percentage));
        }
    }

    for(const auto & snapshot : solver.num_mutants_snapshots) {
        assert(std::get<0>(snapshot) >= 0);
        assert(std::get<0>(snapshot) <= solver.num_iterations);
        assert(std::get<1>(snapshot) >= 0.0);
        assert(std::get<1>(snapshot) <= solver.solving_time);
        assert(std::get<2>(snapshot).size() > 0);
        assert(std::get<2>(snapshot).size() <= solver.max_num_solutions);

        for(const unsigned & num_mutants : std::get<2>(snapshot)) {
            assert(num_mutants >= floor(solver.population_size
                                      * solver.min_mutants_percentage));
            assert(num_mutants <= ceil(solver.population_size
                                     * solver.max_mutants_percentage));
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
              << std::endl;

    std::cout << "Num elites snapshots: ";
    for(unsigned i = 0; i < solver.num_elites_snapshots.size() - 1; i++) {
        std::cout << std::get<2>(solver.num_elites_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_elites_snapshots.back()).front()
              << std::endl;

    std::cout << "Num mutants snapshots: ";
    for(unsigned i = 0; i < solver.num_mutants_snapshots.size() - 1; i++) {
        std::cout << std::get<2>(solver.num_mutants_snapshots[i]).front()
                  << " ";
    }
    std::cout << std::get<2>(solver.num_mutants_snapshots.back()).front()
              << std::endl;

    std::cout << std::endl << "NSMPBRKGA Solver Test PASSED" << std::endl;

    return 0;
}

