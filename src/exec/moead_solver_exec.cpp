#include "utils/argument_parser.hpp"
#include "solver/weighted_sum/christofides/christofides_solver.hpp"
#include "solver/weighted_sum/branch_and_cut/branch_and_cut_solver.hpp"
#include "solver/moead/moead_solver.hpp"

int main (int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if(arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        motsp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));
        ifs >> instance;
        ifs.close();

        unsigned initial_individuals_method = 0;
        double initial_individuals_percentage = 0.0;
        double initial_individuals_time_percentage = 0.0;
        double initial_time = 0.0;
        unsigned initial_iterations = 0;
        std::vector<std::tuple<unsigned, double,
            std::vector<std::vector<double>>>> initial_pareto_snapshots;
        std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
            initial_non_dominated_snapshots;
        std::vector<std::tuple<unsigned, double, std::vector<unsigned>>>
            initial_fronts_snapshots;

        motsp::MOEAD_Solver solver(instance);

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

        if(arg_parser.option_exists("--weight-generation")) {
            solver.weight_generation =
                arg_parser.option_value("--weight-generation");
        }

        if(arg_parser.option_exists("--decomposition")) {
            solver.decomposition = arg_parser.option_value("--decomposition");
        }

        if(arg_parser.option_exists("--neighbours")) {
            solver.neighbours =
                std::stoul(arg_parser.option_value("--neighbours"));
        }

        if(arg_parser.option_exists("--cr")) {
            solver.cr = std::stod(arg_parser.option_value("--cr"));
        }

        if(arg_parser.option_exists("--f")) {
            solver.f = std::stod(arg_parser.option_value("--f"));
        }

        if(arg_parser.option_exists("--eta-m")) {
            solver.eta_m = std::stod(arg_parser.option_value("--eta-m"));
        }

        if(arg_parser.option_exists("--realb")) {
            solver.realb = std::stod(arg_parser.option_value("--realb"));
        }

        if(arg_parser.option_exists("--limit")) {
            solver.limit = std::stoul(arg_parser.option_value("--limit"));
        }

        solver.preserve_diversity =
            arg_parser.option_exists("--preserve-diversity");

        if(arg_parser.option_exists("--initial-individuals-method")) {
            initial_individuals_method = std::stoul(
                    arg_parser.option_value("--initial-individuals-method"));
        }

        if(arg_parser.option_exists("--initial-individuals-time-percentage")) {
            initial_individuals_time_percentage =
                std::stod(arg_parser.option_value(
                            "--initial-individuals-time-percentage"));
        }

        if(arg_parser.option_exists("--initial-individuals-percentage")) {
            initial_individuals_percentage = std::stod(arg_parser.option_value(
                        "--initial-individuals-percentage"));
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
            initial_pareto_snapshots = initial_solver.pareto_snapshots;
            initial_non_dominated_snapshots =
                initial_solver.non_dominated_snapshots;
            initial_fronts_snapshots = initial_solver.fronts_snapshots;
            solver.num_snapshots = initial_solver.num_snapshots;
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
            initial_pareto_snapshots = initial_solver.pareto_snapshots;
            initial_non_dominated_snapshots =
                initial_solver.non_dominated_snapshots;
            initial_fronts_snapshots = initial_solver.fronts_snapshots;
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
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                                         arg_parser.option_value("--pareto") +
                                         " not created.");
            }
        }

        if(arg_parser.option_exists("--pareto-snapshots")) {
            std::string pareto_snapshots_filename =
                arg_parser.option_value("--pareto-snapshots");

            std::transform(solver.pareto_snapshots.begin(),
                           solver.pareto_snapshots.end(),
                           solver.pareto_snapshots.begin(),
                           [&](const auto & snapshot) {
                                auto result = snapshot;
                                std::get<0>(result) += initial_iterations;
                                std::get<1>(result) += initial_time;
                                return result;
                           });

            initial_pareto_snapshots.insert(initial_pareto_snapshots.end(),
                                            solver.pareto_snapshots.begin(),
                                            solver.pareto_snapshots.end());

            solver.pareto_snapshots = initial_pareto_snapshots;

            for(unsigned i = 0; i < solver.pareto_snapshots.size(); i++) {
                std::ofstream ofs;
                ofs.open(pareto_snapshots_filename + std::to_string(i) +
                        ".txt");

                if(ofs.is_open()) {
                    unsigned iteration =
                        std::get<0>(solver.pareto_snapshots[i]);
                    double time = std::get<1>(solver.pareto_snapshots[i]);
                    std::vector<std::vector<double>> pareto =
                        std::get<2>(solver.pareto_snapshots[i]);

                    ofs << iteration << " "
                        << time << std::endl;

                    for(unsigned j = 0; j < pareto.size(); j++) {
                        for(unsigned k = 0;
                            k < pareto[j].size() - 1;
                            k++) {
                            ofs << pareto[j][k] << " ";
                        }

                        ofs << pareto[j].back() << std::endl;
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                                             pareto_snapshots_filename +
                                             std::to_string(i) +
                                             ".txt not created.");
                }
            }
        }

        if(arg_parser.option_exists("--non-dominated-snapshots")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--non-dominated-snapshots"));

            if(ofs.is_open()) {
                std::transform(solver.non_dominated_snapshots.begin(),
                               solver.non_dominated_snapshots.end(),
                               solver.non_dominated_snapshots.begin(),
                               [&](const auto & snapshot) {
                                    auto result = snapshot;
                                    std::get<0>(result) += initial_iterations;
                                    std::get<1>(result) += initial_time;
                                    return result;
                               });

                initial_non_dominated_snapshots.insert(
                        initial_non_dominated_snapshots.end(),
                        solver.non_dominated_snapshots.begin(),
                        solver.non_dominated_snapshots.end());

                solver.non_dominated_snapshots =
                    initial_non_dominated_snapshots;

                for(unsigned i = 0;
                    i < solver.non_dominated_snapshots.size();
                    i++) {
                    unsigned iteration =
                        std::get<0>(solver.non_dominated_snapshots[i]);
                    double time =
                        std::get<1>(solver.non_dominated_snapshots[i]);
                    std::vector<unsigned> non_dominated =
                        std::get<2>(solver.non_dominated_snapshots[i]);

                    ofs << iteration << " "
                        << time << " ";

                    for(unsigned j = 0; j < non_dominated.size() - 1; j++) {
                        ofs << non_dominated[j] << " ";
                    }

                    ofs << non_dominated.back() << std::endl;
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " +
                        arg_parser.option_value("--non-dominated-snapshots") +
                        " not created.");
            }
        }

        if(arg_parser.option_exists("--fronts-snapshots")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--fronts-snapshots"));

            if(ofs.is_open()) {
                std::transform(solver.fronts_snapshots.begin(),
                               solver.fronts_snapshots.end(),
                               solver.fronts_snapshots.begin(),
                               [&](const auto & snapshot) {
                                    auto result = snapshot;
                                    std::get<0>(result) += initial_iterations;
                                    std::get<1>(result) += initial_time;
                                    return result;
                               });

                initial_fronts_snapshots.insert(initial_fronts_snapshots.end(),
                                                solver.fronts_snapshots.begin(),
                                                solver.fronts_snapshots.end());

                solver.fronts_snapshots = initial_fronts_snapshots;

                for(unsigned i = 0; i < solver.fronts_snapshots.size(); i++) {
                    unsigned iteration =
                        std::get<0>(solver.fronts_snapshots[i]);
                    double time = std::get<1>(solver.fronts_snapshots[i]);
                    std::vector<unsigned> num_fronts =
                        std::get<2>(solver.fronts_snapshots[i]);

                    ofs << iteration << " "
                        << time << " ";

                    for(unsigned j = 0; j < num_fronts.size() - 1; j++) {
                        ofs << num_fronts[j] << " ";
                    }

                    ofs << num_fronts.back() << std::endl;
                }

                ofs.close();
            } else {
                throw std::runtime_error(
                        "File " +
                        arg_parser.option_value("--fronts-snapshots") +
                        " not created.");
            }
        }
    } else {
        std::cerr << "./moead_solver_exec "
                  << "--instance <instance_filename> "
                  << "--seed <seed> "
                  << "--time-limit <time_limit> "
                  << "--iterations-limit <iterations_limit> "
                  << "--max-num-solutions <max_num_solutions> "
                  << "--max-num-snapshots <max_num_snapshots> "
                  << "--population-size <population_size> "
                  << "--weight-generation <weight_generation> "
                  << "--decomposition <decomposition> "
                  << "--neighbours <neighbours> "
                  << "--cr <cr> "
                  << "--f <f> "
                  << "--eta-m <eta_m> "
                  << "--realb <realb> "
                  << "--limit <limit> "
                  << "--preserve-diversity "
                  << "--initial-individuals-method <initial_individuals_method> "
                  << "--initial-individuals-percentage <initial_individuals_percentage> "
                  << "--initial-individuals-time-percentage <initial_individuals_time_percentage> "
                  << "--statistics <statistics_filename> "
                  << "--solutions <solutions_filename> "
                  << "--pareto <pareto_filename> "
                  << "--pareto-snapshots <pareto_snapshots_filename> "
                  << "--non-dominated-snapshots <non_dominated_snapshots_filename> "
                  << "--fronts-snapshots <fronts_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}

