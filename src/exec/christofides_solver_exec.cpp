#include "utils/argument_parser.hpp"
#include "solver/weighted_sum/christofides/christofides_solver.hpp"

int main (int argc, char * argv[]) {
    Argument_Parser arg_parser (argc, argv);

    if(arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        motsp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));
        ifs >> instance;
        ifs.close();

        motsp::Christofides_Solver solver(instance);

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
                arg_parser.option_exists("--max-num-snapshots");
        }

        solver.solve();

        if(arg_parser.option_exists("--statistics")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--statistics"));
            ofs << solver;
            ofs.close();
        }

        if(arg_parser.option_exists("--solutions")) {
            std::string solution_filename =
                arg_parser.option_value("--solutions");

            for(unsigned i = 0; i < solver.best_solutions.size(); i++) {
                std::ofstream ofs;
                ofs.open(solution_filename + std::to_string(i) + ".sol");
                ofs << solver.best_solutions[i];
                ofs.close();
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
                throw "File not created.";
            }
        }

        if(arg_parser.option_exists("--best-solutions-snapshots")) {
            std::string best_solutions_snapshots_filename =
                arg_parser.option_value("--best-solutions-snapshots");

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

                    ofs.close();
                } else {
                    throw "File not created.";
                }
            }
        }
    } else {
        std::cerr << "./christofides_solver_exec "
                  << "--instance <instance_filename> "
                  << "--seed <seed> "
                  << "--time-limit <time_limit> "
                  << "--iterations-limit <iterations_limit> "
                  << "--max-num-solutions <max_num_solutions> "
                  << "--max-num-snapshots <max_num_snapshots> "
                  << "--statistics <statistics_filename> "
                  << "--solutions <solutions_filename> "
                  << "--pareto <pareto_filename> "
                  << "--best-solutions-snapshots <best_solutions_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}

