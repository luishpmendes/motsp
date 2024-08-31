#include "utils/argument_parser.hpp"
#include "solver/weighted_sum/christofides/christofides_solver.hpp"
#include "solver/weighted_sum/branch_and_cut/branch_and_cut_solver.hpp"
#include "solver/nsbrkga/nsbrkga_solver.hpp"

int main (int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if(arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        motsp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));

        if(ifs.is_open()) {
            ifs >> instance;

            if(ifs.eof() || ifs.fail() || ifs.bad()) {
                throw std::runtime_error("Error reading file " +
                        arg_parser.option_value("--instance") + ".");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--instance") + " not found.");
        }

        unsigned initial_individuals_method = 0;
        double initial_individuals_percentage = 0.0;
        double initial_individuals_time_percentage = 0.0;
        double initial_time = 0.0;
        unsigned initial_iterations = 0;
        std::vector<std::tuple<unsigned, double,
            std::vector<std::vector<double>>>> initial_best_solutions_snapshots;
        std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
            initial_num_non_dominated_snapshots;
        std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
            initial_num_fronts_snapshots;
        std::vector<std::tuple<unsigned, double,
            std::vector<std::vector<std::vector<double>>>>>
                initial_populations_snapshots;

        motsp::NSBRKGA_Solver solver(instance);

        if(arg_parser.option_exists("--seed")) {
            solver.set_seed(std::stoul(arg_parser.option_value("--seed")));
        }

        if(arg_parser.option_exists("--time-limit")) {
            solver.time_limit =
                std::stod(arg_parser.option_value("--time-limit"));
        }

        if(arg_parser.option_exists("--iterations-limit")) {
            solver.iterations_limit =
                std::stoul(arg_parser.option_value("--iterations-limit"));
        }

        if(arg_parser.option_exists("--max-num-solutions")) {
            solver.max_num_solutions =
                std::stoul(arg_parser.option_value("--max-num-solutions"));
        }

        if(arg_parser.option_exists("--max-num-snapshots")) {
            solver.max_num_snapshots =
                std::stoul(arg_parser.option_value("--max-num-snapshots"));
        }

        if(arg_parser.option_exists("--population-size")) {
            solver.population_size =
                std::stoul(arg_parser.option_value("--population-size"));
        }

        if(arg_parser.option_exists("--min-elites-percentage")) {
            solver.min_elites_percentage =
                std::stod(arg_parser.option_value("--min-elites-percentage"));
        }

        if(arg_parser.option_exists("--max-elites-percentage")) {
            solver.max_elites_percentage =
                std::stod(arg_parser.option_value("--max-elites-percentage"));
        }

        if(arg_parser.option_exists("--mutation-probability")) {
            solver.mutation_probability =
                std::stod(arg_parser.option_value("--mutation-probability"));
        }

        if(arg_parser.option_exists("--mutation-distribution")) {
            solver.mutation_distribution =
                std::stod(arg_parser.option_value("--mutation-distribution"));
        }

        if(arg_parser.option_exists("--num-total-parents")) {
            solver.num_total_parents =
                std::stoul(arg_parser.option_value("--num-total-parents"));
        }

        if(arg_parser.option_exists("--num-elite-parents")) {
            solver.num_elite_parents =
                std::stoul(arg_parser.option_value("--num-elite-parents"));
        }

        if(arg_parser.option_exists("--bias-type")) {
            std::stringstream ss(arg_parser.option_value("--bias-type"));
            ss >> solver.bias_type;
        }

        if(arg_parser.option_exists("--diversity-type")) {
            std::stringstream ss(arg_parser.option_value("--diversity-type"));
            ss >> solver.diversity_type;
        }

        if(arg_parser.option_exists("--num-populations")) {
            solver.num_populations =
                std::stoul(arg_parser.option_value("--num-populations"));
        }

        if(arg_parser.option_exists("--exchange-interval")) {
            solver.exchange_interval =
                std::stoul(arg_parser.option_value("--exchange-interval"));
        }

        if(arg_parser.option_exists("--num-exchange-individuals")) {
            solver.num_exchange_individuals =
                std::stoul(arg_parser.option_value(
                    "--num-exchange-individuals"));
        }

        if(arg_parser.option_exists("--pr-type")) {
            std::stringstream ss(arg_parser.option_value("--pr-type"));
            ss >> solver.pr_type;
        }

        if(arg_parser.option_exists("--pr-dist-func")) {
            std::string s = arg_parser.option_value("--pr-dist-func");
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);

            if (s.compare("HAMMING") == 0) {
                solver.pr_dist_func =
                    std::shared_ptr<NSBRKGA::DistanceFunctionBase>(
                        new NSBRKGA::KendallTauDistance());
            } else if (s.compare("KENDALL_TAU") == 0) {
                solver.pr_dist_func =
                    std::shared_ptr<NSBRKGA::DistanceFunctionBase>(
                        new NSBRKGA::KendallTauDistance());
            } else if (s.compare("EUCLIDEAN") == 0) {
                solver.pr_dist_func =
                    std::shared_ptr<NSBRKGA::DistanceFunctionBase>(
                        new NSBRKGA::EuclideanDistance());
            }
        }

        if(arg_parser.option_exists("--pr-percentage")) {
            solver.pr_percentage =
                std::stod(arg_parser.option_value("--pr-percentage"));
        }

        if(arg_parser.option_exists("--pr-interval")) {
            solver.pr_interval =
                std::stoul(arg_parser.option_value("--pr-interval"));
        }

        if(arg_parser.option_exists("--shake-interval")) {
            solver.shake_interval =
                std::stoul(arg_parser.option_value("--shake-interval"));
        }

        if(arg_parser.option_exists("--shake-intensity")) {
            solver.shake_intensity =
                std::stod(arg_parser.option_value("--shake-intensity"));
        }

        if(arg_parser.option_exists("--shake-distribution")) {
            solver.shake_distribution =
                std::stod(arg_parser.option_value("--shake-distribution"));
        }

        if(arg_parser.option_exists("--reset-interval")) {
            solver.reset_interval =
                std::stoul(arg_parser.option_value("--reset-interval"));
        }

        if(arg_parser.option_exists("--reset-intensity")) {
            solver.reset_intensity =
                std::stod(arg_parser.option_value("--reset-intensity"));
        }

        if(arg_parser.option_exists("--num-threads")) {
            solver.num_threads =
                std::stoul(arg_parser.option_value("--num-threads"));
        }

        if(arg_parser.option_exists("--max-local-search-iterations")) {
            solver.max_local_search_iterations = std::stoul(
                    arg_parser.option_value("--max-local-search-iterations"));
        }

        if(arg_parser.option_exists("--initial-individuals-method")) {
            initial_individuals_method = std::stoul(
                    arg_parser.option_value("--initial-individuals-method"));
        }

        if(arg_parser.option_exists("--initial-individuals-percentage")) {
            initial_individuals_percentage = std::stod(arg_parser.option_value(
                        "--initial-individuals-percentage"));
        }

        if(arg_parser.option_exists("--initial-individuals-time-percentage")) {
            initial_individuals_time_percentage = std::stod(
                    arg_parser.option_value(
                        "--initial-individuals-time-percentage"));
        }

        if(initial_individuals_method == 1) {
            motsp::Christofides_Solver initial_solver(instance);

            initial_solver.set_seed(solver.seed);
            initial_solver.time_limit = double(solver.time_limit) *
                initial_individuals_time_percentage;
            initial_solver.iterations_limit = double(solver.population_size) *
                initial_individuals_percentage;
            initial_solver.max_num_solutions = double(solver.population_size) *
                initial_individuals_percentage;
            initial_solver.max_num_snapshots = double(solver.max_num_snapshots)
                * initial_individuals_percentage;

            initial_solver.solve();

            solver.initial_individuals = initial_solver.best_individuals;
            initial_time = initial_solver.solving_time;
            initial_iterations = initial_solver.num_iterations;
            initial_best_solutions_snapshots =
                initial_solver.best_solutions_snapshots;
            initial_num_non_dominated_snapshots =
                initial_solver.num_non_dominated_snapshots;
            initial_num_fronts_snapshots = initial_solver.num_fronts_snapshots;
            initial_populations_snapshots =
                initial_solver.populations_snapshots;
            solver.num_snapshots = initial_solver.num_snapshots;
            solver.max_num_snapshots -= initial_solver.num_snapshots;
        } else if (initial_individuals_method == 2) {
            motsp::Branch_and_Cut_Solver initial_solver(instance);

            initial_solver.set_seed(solver.seed);
            initial_solver.time_limit = double(solver.time_limit) *
                initial_individuals_time_percentage;
            initial_solver.iterations_limit = double(solver.population_size) *
                initial_individuals_percentage;
            initial_solver.max_num_solutions = double(solver.population_size) *
                initial_individuals_percentage;
            initial_solver.max_num_snapshots = double(solver.max_num_snapshots)
                * initial_individuals_percentage;

            initial_solver.solve();

            solver.initial_individuals = initial_solver.best_individuals;
            initial_time = initial_solver.solving_time;
            initial_iterations = initial_solver.num_iterations;
            initial_best_solutions_snapshots =
                initial_solver.best_solutions_snapshots;
            initial_num_non_dominated_snapshots =
                initial_solver.num_non_dominated_snapshots;
            initial_num_fronts_snapshots = initial_solver.num_fronts_snapshots;
            initial_populations_snapshots =
                initial_solver.populations_snapshots;
            solver.num_snapshots = initial_solver.num_snapshots;
            solver.max_num_snapshots -= initial_solver.num_snapshots;
        }

        solver.time_limit -= initial_time;
        solver.iterations_limit -= initial_iterations;

        solver.solve();

        if(arg_parser.option_exists("--statistics")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--statistics"));

            if(ofs.is_open()) {
                ofs << solver;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--statistics") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " + arg_parser.option_value("--statistics") +
                        " not created.");
            }
        }

        if(arg_parser.option_exists("--solutions")) {
            std::string solution_filename =
                arg_parser.option_value("--solutions");

            for(unsigned i = 0; i < solver.best_solutions.size(); i++) {
                std::ofstream ofs;
                ofs.open(solution_filename + std::to_string(i) + ".sol");

                if(ofs.is_open()) {
                    ofs << solver.best_solutions[i];

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                solution_filename + std::to_string(i) +
                                ".sol.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " + solution_filename +
                                             std::to_string(i) +
                                             ".sol not created.");
                }
            }
        }

        if(arg_parser.option_exists("--pareto")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--pareto"));

            if(ofs.is_open()) {
                for(const auto & solution : solver.best_solutions) {
                    for(unsigned i = 0; i < solution.cost.size() - 1; i++) {
                        ofs << solution.cost[i] << " ";
                    }

                    ofs << solution.cost.back() << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value("--pareto") + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                                         arg_parser.option_value("--pareto") +
                                         " not created.");
            }
        }

        if(arg_parser.option_exists("--best-solutions-snapshots")) {
            std::string best_solutions_snapshots_filename =
                arg_parser.option_value("--best-solutions-snapshots");

            std::transform(solver.best_solutions_snapshots.begin(),
                           solver.best_solutions_snapshots.end(),
                           solver.best_solutions_snapshots.begin(),
                           [&](const auto & snapshot) {
                                auto result = snapshot;
                                std::get<0>(result) += initial_iterations;
                                std::get<1>(result) += initial_time;
                                return result;
                           });

            initial_best_solutions_snapshots.insert(
                    initial_best_solutions_snapshots.end(),
                    solver.best_solutions_snapshots.begin(),
                    solver.best_solutions_snapshots.end());

            solver.best_solutions_snapshots = initial_best_solutions_snapshots;

            for(unsigned i = 0;
                i < solver.best_solutions_snapshots.size();
                i++) {
                std::ofstream ofs;
                ofs.open(best_solutions_snapshots_filename + std::to_string(i)
                        + ".txt");

                if(ofs.is_open()) {
                    unsigned iteration =
                        std::get<0>(solver.best_solutions_snapshots[i]);
                    double time =
                        std::get<1>(solver.best_solutions_snapshots[i]);
                    std::vector<std::vector<double>> best_solutions =
                        std::get<2>(solver.best_solutions_snapshots[i]);

                    ofs << iteration << " "
                        << time << std::endl;

                    for(unsigned j = 0; j < best_solutions.size(); j++) {
                        for(unsigned k = 0;
                            k < best_solutions[j].size() - 1;
                            k++) {
                            ofs << best_solutions[j][k] << " ";
                        }

                        ofs << best_solutions[j].back() << std::endl;
                    }

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                best_solutions_snapshots_filename +
                                std::to_string(i) + "txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error(
                            "File " +
                            best_solutions_snapshots_filename +
                            std::to_string(i) +
                            ".txt not created.");
                }
            }
        }

        if(arg_parser.option_exists("--num-non-dominated-snapshots")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-non-dominated-snapshots"));

            if(ofs.is_open()) {
                std::transform(solver.num_non_dominated_snapshots.begin(),
                               solver.num_non_dominated_snapshots.end(),
                               solver.num_non_dominated_snapshots.begin(),
                               [&](const auto & snapshot) {
                                    auto result = snapshot;
                                    std::get<0>(result) += initial_iterations;
                                    std::get<1>(result) += initial_time;
                                    return result;
                               });

                initial_num_non_dominated_snapshots.insert(
                        initial_num_non_dominated_snapshots.end(),
                        solver.num_non_dominated_snapshots.begin(),
                        solver.num_non_dominated_snapshots.end());

                solver.num_non_dominated_snapshots =
                    initial_num_non_dominated_snapshots;

                for(unsigned i = 0;
                    i < solver.num_non_dominated_snapshots.size();
                    i++) {
                    unsigned iteration =
                        std::get<0>(solver.num_non_dominated_snapshots[i]);
                    double time =
                        std::get<1>(solver.num_non_dominated_snapshots[i]);
                    std::vector<unsigned> num_non_dominated =
                        std::get<2>(solver.num_non_dominated_snapshots[i]);

                    ofs << iteration << " "
                        << time << " ";

                    for(unsigned j = 0; j < num_non_dominated.size() - 1; j++) {
                        ofs << num_non_dominated[j] << " ";
                    }

                    ofs << num_non_dominated.back() << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--num-non-dominated-snapshots") + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " +
                        arg_parser.option_value(
                            "--num-non-dominated-snapshots") +
                        " not created.");
            }
        }

        if(arg_parser.option_exists("--num-fronts-snapshots")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-fronts-snapshots"));

            if(ofs.is_open()) {
                std::transform(solver.num_fronts_snapshots.begin(),
                               solver.num_fronts_snapshots.end(),
                               solver.num_fronts_snapshots.begin(),
                               [&](const auto & snapshot) {
                                    auto result = snapshot;
                                    std::get<0>(result) += initial_iterations;
                                    std::get<1>(result) += initial_time;
                                    return result;
                               });

                initial_num_fronts_snapshots.insert(
                        initial_num_fronts_snapshots.end(),
                        solver.num_fronts_snapshots.begin(),
                        solver.num_fronts_snapshots.end());

                solver.num_fronts_snapshots = initial_num_fronts_snapshots;

                for(unsigned i = 0;
                    i < solver.num_fronts_snapshots.size();
                    i++) {
                    unsigned iteration =
                        std::get<0>(solver.num_fronts_snapshots[i]);
                    double time = std::get<1>(solver.num_fronts_snapshots[i]);
                    std::vector<unsigned> num_fronts =
                        std::get<2>(solver.num_fronts_snapshots[i]);

                    ofs << iteration << " "
                        << time << " ";

                    for(unsigned j = 0; j < num_fronts.size() - 1; j++) {
                        ofs << num_fronts[j] << " ";
                    }

                    ofs << num_fronts.back() << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--num-fronts-snapshots") + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " +
                        arg_parser.option_value("--num-fronts-snapshots") +
                        " not created.");
            }
        }

        if(arg_parser.option_exists("--populations-snapshots")) {
            std::string populations_snapshots_filename =
                arg_parser.option_value("--populations-snapshots");

            std::transform(solver.populations_snapshots.begin(),
                           solver.populations_snapshots.end(),
                           solver.populations_snapshots.begin(),
                           [&](const auto & snapshot) {
                                auto result = snapshot;
                                std::get<0>(result) += initial_iterations;
                                std::get<1>(result) += initial_time;
                                return result;
                           });

            initial_populations_snapshots.insert(
                    initial_populations_snapshots.end(),
                    solver.populations_snapshots.begin(),
                    solver.populations_snapshots.end());

            solver.populations_snapshots = initial_populations_snapshots;

            for(unsigned i = 0;
                i < solver.populations_snapshots.size();
                i++) {
                std::ofstream ofs;
                ofs.open(populations_snapshots_filename + std::to_string(i) +
                        ".txt");

                if(ofs.is_open()) {
                    unsigned iteration =
                        std::get<0>(solver.populations_snapshots[i]);
                    double time =
                        std::get<1>(solver.populations_snapshots[i]);
                    std::vector<std::vector<std::vector<double>>> population =
                        std::get<2>(solver.populations_snapshots[i]);

                    ofs << iteration << " "
                        << time << std::endl;

                    for (unsigned j = 0; j < population.size(); j++) {
                        for(unsigned k = 0; k < population[j].size(); k++) {
                            for(unsigned l = 0;
                                l < population[j][k].size() - 1;
                                l++) {
                                ofs << population[j][k][l] << " ";
                            }

                            ofs << population[j][k].back() << std::endl;
                        }
                    }

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                populations_snapshots_filename +
                                std::to_string(i) + ".txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                                             populations_snapshots_filename +
                                             std::to_string(i) +
                                             ".txt not created.");
                }
            }
        }

        if(arg_parser.option_exists("--num-elites-snapshots")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-elites-snapshots"));

            if(ofs.is_open()) {
                std::transform(solver.num_elites_snapshots.begin(),
                               solver.num_elites_snapshots.end(),
                               solver.num_elites_snapshots.begin(),
                               [&](const auto & snapshot) {
                                    auto result = snapshot;
                                    std::get<0>(result) += initial_iterations;
                                    std::get<1>(result) += initial_time;
                                    return result;
                               });

                for(unsigned i = 0;
                    i < solver.num_elites_snapshots.size();
                    i++) {
                    unsigned iteration =
                        std::get<0>(solver.num_elites_snapshots[i]);
                    double time = std::get<1>(solver.num_elites_snapshots[i]);
                    std::vector<unsigned> num_elites =
                        std::get<2>(solver.num_elites_snapshots[i]);

                    ofs << iteration << " "
                        << time << " ";

                    for(unsigned j = 0; j < num_elites.size() - 1; j++) {
                        ofs << num_elites[j] << " ";
                    }

                    ofs << num_elites.back() << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--num-elites-snapshots") + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " +
                        arg_parser.option_value("--num-elites-snapshots") +
                        " not created.");
            }
        }
    } else {
        std::cerr << "./nsbrkga_solver_exec "
                  << "--instance <instance_filename> "
                  << "--seed <seed> "
                  << "--time-limit <time_limit> "
                  << "--iterations-limit <iterations_limit> "
                  << "--max-num-solutions <max_num_solutions> "
                  << "--max-num-snapshots <max_num_snapshots> "
                  << "--population-size <population_size> "
                  << "--min-elites-percentage <min_elites_percentage> "
                  << "--max-elites-percentage <max_elites_percentage> "
                  << "--mutation-probability <mutation_probability> "
                  << "--mutation-distribution <mutation_distribution> "
                  << "--num-total-parents <num_total_parents> "
                  << "--num-elite-parents <num_elite_parents> "
                  << "--bias-type <bias_type> "
                  << "--diversity-type <diversity_type> "
                  << "--num-populations <num_populations> "
                  << "--exchange-interval <exchange_interval> "
                  << "--num-exchange-individuals <num_exchange_individuals> "
                  << "--pr-type <pr_type> "
                  << "--pr-dist-func <pr_dist_func> "
                  << "--pr-percentage <pr_percentage> "
                  << "--pr-interval <pr_interval> "
                  << "--shake-interval <shake_interval> "
                  << "--shake-intensity <shake_intensity> "
                  << "--shake-distribution <shake_distribution> "
                  << "--reset-interval <reset_interval> "
                  << "--reset-intensity <reset_intensity> "
                  << "--num-threads <num_threads> "
                  << "--max-local-search-iterations <max_local_search_iterations> "
                  << "--initial-individuals-method <initial_individuals_method> "
                  << "--initial-individuals-percentage <initial_individuals_percentage> "
                  << "--initial-individuals-time-percentage <initial_individuals_time_percentage> "
                  << "--statistics <statistics_filename> "
                  << "--solutions <solutions_filename> "
                  << "--pareto <pareto_filename> "
                  << "--best-solutions-snapshots <best_solutions_snapshots_filename> "
                  << "--num-non-dominated-snapshots <num_non_dominated_snapshots_filename> "
                  << "--num-fronts-snapshots <num_fronts_snapshots_filename> "
                  << "--populations-snapshots <populations_snapshots_filename> "
                  << "--num-elites-snapshots <num_elites_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}
