#include "solver/nsbrkga/nsbrkga_solver.hpp"
#include <cassert>

int main() {
    std::ifstream ifs;
    motsp::Instance instance;
    motsp::NSBRKGA_Solver solver;

    std::cout << "instances/kroAC100.txt" << std::endl;

    ifs.open("instances/kroAC100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSBRKGA_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 1000;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;
    solver.exchange_interval = 20;
    solver.num_exchange_individuals = 3;
    solver.pr_interval = 50;
    solver.shake_interval = 20;
    solver.reset_interval = 50;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 1000);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.min_elites_percentage - 0.10) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.max_elites_percentage - 0.30) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_total_parents == 3);
    assert(solver.num_elite_parents == 2);
    assert(solver.bias_type == NSBRKGA::BiasFunctionType::SQRT);
    assert(solver.diversity_type ==
            NSBRKGA::DiversityFunctionType::AVERAGE_DISTANCE_TO_CENTROID);
    assert(solver.num_populations == 3);
    assert(solver.exchange_interval == 20);
    assert(solver.num_exchange_individuals == 3);
    assert(solver.pr_type == NSBRKGA::PathRelinking::Type::BINARY_SEARCH);
    assert(typeid(*solver.pr_dist_func) == typeid(NSBRKGA::EuclideanDistance));
    assert(fabs(solver.pr_percentage - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.pr_interval == 50);
    assert(solver.shake_interval == 20);
    assert(fabs(solver.shake_intensity - 0.33) < 
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.shake_distribution - 20.0) < 
            std::numeric_limits<double>::epsilon());
    assert(solver.reset_interval == 50);
    assert(fabs(solver.reset_intensity - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_threads == 1);

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
    assert(solver.num_elites_snapshots.size() == solver.num_snapshots);

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

    std::cout << "Num elites snapshots: ";
    for(unsigned i = 0; i < solver.num_elites_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_elites_snapshots[i]).begin(),
            std::get<2>(solver.num_elites_snapshots[i]).end(),
            0) / std::get<2>(solver.num_elites_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_elites_snapshots.back())
              << ", "
              << std::get<1>(solver.num_elites_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_elites_snapshots.back()).begin(),
        std::get<2>(solver.num_elites_snapshots.back()).end(),
        0) / std::get<2>(solver.num_elites_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "instances/kroAD100.txt" << std::endl;

    ifs.open("instances/kroAD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSBRKGA_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 1000;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;
    solver.exchange_interval = 20;
    solver.num_exchange_individuals = 3;
    solver.pr_interval = 50;
    solver.shake_interval = 20;
    solver.reset_interval = 50;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 1000);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.min_elites_percentage - 0.10) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.max_elites_percentage - 0.30) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_total_parents == 3);
    assert(solver.num_elite_parents == 2);
    assert(solver.bias_type == NSBRKGA::BiasFunctionType::SQRT);
    assert(solver.diversity_type ==
            NSBRKGA::DiversityFunctionType::AVERAGE_DISTANCE_TO_CENTROID);
    assert(solver.num_populations == 3);
    assert(solver.exchange_interval == 20);
    assert(solver.num_exchange_individuals == 3);
    assert(solver.pr_type == NSBRKGA::PathRelinking::Type::BINARY_SEARCH);
    assert(typeid(*solver.pr_dist_func) == typeid(NSBRKGA::EuclideanDistance));
    assert(fabs(solver.pr_percentage - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.pr_interval == 50);
    assert(solver.shake_interval == 20);
    assert(fabs(solver.shake_intensity - 0.33) < 
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.shake_distribution - 20.0) < 
            std::numeric_limits<double>::epsilon());
    assert(solver.reset_interval == 50);
    assert(fabs(solver.reset_intensity - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_threads == 1);

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
    assert(solver.num_elites_snapshots.size() == solver.num_snapshots);

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
                assert(s[1] >= 21294);
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

    std::cout << "Num elites snapshots: ";
    for(unsigned i = 0; i < solver.num_elites_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_elites_snapshots[i]).begin(),
            std::get<2>(solver.num_elites_snapshots[i]).end(),
            0) / std::get<2>(solver.num_elites_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_elites_snapshots.back())
              << ", "
              << std::get<1>(solver.num_elites_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_elites_snapshots.back()).begin(),
        std::get<2>(solver.num_elites_snapshots.back()).end(),
        0) / std::get<2>(solver.num_elites_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << "instances/kroCD100.txt" << std::endl;

    ifs.open("instances/kroCD100.txt");
    assert(ifs.is_open());
    ifs >> instance;
    ifs.close();

    solver = motsp::NSBRKGA_Solver(instance);

    solver.set_seed(2351389233);
    solver.time_limit = 5.0;
    solver.iterations_limit = 1000;
    solver.max_num_solutions = 64;
    solver.population_size = 32;
    solver.max_num_snapshots = 16;
    solver.exchange_interval = 20;
    solver.num_exchange_individuals = 3;
    solver.pr_interval = 50;
    solver.shake_interval = 20;
    solver.reset_interval = 50;

    assert((solver.seed = 2351389233));
    assert(fabs(solver.time_limit - 5.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.iterations_limit == 1000);
    assert(solver.max_num_solutions == 64);
    assert(solver.population_size == 32);
    assert(solver.max_num_snapshots == 16);
    assert(fabs(solver.min_elites_percentage - 0.10) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.max_elites_percentage - 0.30) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_probability - 0.01) <
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.mutation_distribution - 50.0) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_total_parents == 3);
    assert(solver.num_elite_parents == 2);
    assert(solver.bias_type == NSBRKGA::BiasFunctionType::SQRT);
    assert(solver.diversity_type ==
            NSBRKGA::DiversityFunctionType::AVERAGE_DISTANCE_TO_CENTROID);
    assert(solver.num_populations == 3);
    assert(solver.exchange_interval == 20);
    assert(solver.num_exchange_individuals == 3);
    assert(solver.pr_type == NSBRKGA::PathRelinking::Type::BINARY_SEARCH);
    assert(typeid(*solver.pr_dist_func) == typeid(NSBRKGA::EuclideanDistance));
    assert(fabs(solver.pr_percentage - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.pr_interval == 50);
    assert(solver.shake_interval == 20);
    assert(fabs(solver.shake_intensity - 0.33) < 
            std::numeric_limits<double>::epsilon());
    assert(fabs(solver.shake_distribution - 20.0) < 
            std::numeric_limits<double>::epsilon());
    assert(solver.reset_interval == 50);
    assert(fabs(solver.reset_intensity - 0.20) <
            std::numeric_limits<double>::epsilon());
    assert(solver.num_threads == 1);

    solver.solve();

    assert(solver.solving_time > 0);

    assert(solver.num_iterations > 0);
    assert(solver.num_iterations <= solver.iterations_limit);

    assert(solver.best_solutions.size() > 0);
    assert(solver.best_solutions.size() <= solver.max_num_solutions);

    assert(solver.num_snapshots > 0);
    assert(solver.num_snapshots == solver.max_num_snapshots);

    assert(solver.best_solutions_snapshots.size() == solver.num_snapshots);
    assert(solver.num_non_dominated_snapshots.size() == solver.num_snapshots);
    assert(solver.num_fronts_snapshots.size() == solver.num_snapshots);
    assert(solver.populations_snapshots.size() == solver.num_snapshots);
    assert(solver.num_elites_snapshots.size() == solver.num_snapshots);

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
                assert(s[0] >= 20749);
                assert(s[1] >= 21294);
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

    std::cout << "Num elites snapshots: ";
    for(unsigned i = 0; i < solver.num_elites_snapshots.size() - 1; i++) {
        std::cout << "("
                  << std::get<0>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::get<1>(solver.num_elites_snapshots[i])
                  << ", "
                  << std::accumulate(
            std::get<2>(solver.num_elites_snapshots[i]).begin(),
            std::get<2>(solver.num_elites_snapshots[i]).end(),
            0) / std::get<2>(solver.num_elites_snapshots[i]).size()
                  << "), ";
    }
    std::cout << "("
              << std::get<0>(solver.num_elites_snapshots.back())
              << ", "
              << std::get<1>(solver.num_elites_snapshots.back())
              << ", "
              << std::accumulate(
        std::get<2>(solver.num_elites_snapshots.back()).begin(),
        std::get<2>(solver.num_elites_snapshots.back()).end(),
        0) / std::get<2>(solver.num_elites_snapshots.back()).size()
              << ")" << std::endl;

    std::cout << std::endl << "NSBRKGA Solver Test PASSED" << std::endl;

    return 0;
}
