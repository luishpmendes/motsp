#include "instance/instance.hpp"
#include "solver/solver.hpp"
#include "utils/argument_parser.hpp"
#include <sstream>
#include <fstream>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if(arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        motsp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));

        if(ifs.is_open()) {
            ifs >> instance;

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--instance") + " not found.");
        }

        std::vector<std::pair<std::vector<double>, std::vector<double>>>
                reference_pareto,
                pareto,
                best_solutions_snapshot;
        unsigned num_solvers, max_num_solutions = 800;

        if(arg_parser.option_exists("--max-num-solutions")) {
            max_num_solutions =
                std::stoul(arg_parser.option_value("--max-num-solutions"));
        }

        for(num_solvers = 0;
            arg_parser.option_exists("--pareto-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--best-solutions-snapshots-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--hypervolume-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--hypervolume-snapshots-" +
                                     std::to_string(num_solvers));
            num_solvers++) {}

        for(unsigned i = 0; i < num_solvers; i++) {
            if(arg_parser.option_exists("--pareto-" + std::to_string(i))) {
                ifs.open(arg_parser.option_value("--pareto-" +
                                                 std::to_string(i)));

                if(ifs.is_open()) {
                    pareto.clear();

                    for(std::string line; std::getline(ifs, line);) {
                        std::istringstream iss(line);
                        std::vector<double> value(instance.num_objectives, 0.0);

                        for(unsigned j = 0; j < instance.num_objectives; j++) {
                            iss >> value[j];
                        }

                        pareto.push_back(std::make_pair(value, std::vector<double>()));
                    }

                    motsp::Solver::update_best_individuals(reference_pareto,
                                                           pareto,
                                                           instance.senses,
                                                           max_num_solutions);

                    ifs.close();
                } else {
                    throw std::runtime_error("File " +
                            arg_parser.option_value("--pareto-" +
                                std::to_string(i)) + " not found.");
                }
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            if(arg_parser.option_exists("--best-solutions-snapshots-" +
                                        std::to_string(i))) {
                std::string best_solutions_snapshots_filename =
                    arg_parser.option_value("--best-solutions-snapshots-" +
                                            std::to_string(i));

                for(unsigned j = 0; ; j++) {
                    ifs.open(best_solutions_snapshots_filename +
                             std::to_string(j) + ".txt");

                    if(ifs.is_open()) {
                        unsigned iteration;
                        double time;

                        ifs >> iteration >> time;

                        best_solutions_snapshot.clear();

                        ifs.ignore();

                        for(std::string line; std::getline(ifs, line);) {
                            std::istringstream iss(line);
                            std::vector<double> value(instance.num_objectives, 0.0);

                            for(unsigned j = 0; j < instance.num_objectives; j++) {
                                iss >> value[j];
                            }

                            best_solutions_snapshot.push_back(
                                std::make_pair(value, std::vector<double>()));
                        }

                        motsp::Solver::update_best_individuals(
                            reference_pareto,
                            best_solutions_snapshot,
                            instance.senses,
                            max_num_solutions);

                        ifs.close();
                    } else {
                        break;
                    }
                }
            }
        }

        // Decrease the value of each objective function by 5%.
        for(std::pair<std::vector<double>, std::vector<double>> & solution : reference_pareto) {
            for(double & value : solution.first) {
                value *= 0.95;
            }
        }

        if(arg_parser.option_exists("--reference-pareto")) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--reference-pareto"));

            if(ofs.is_open()) {
                for(const std::pair<std::vector<double>, std::vector<double>> & solution : reference_pareto) {
                    for(unsigned i = 0; i < solution.first.size() - 1; i++) {
                        ofs << solution.first[i] << " ";
                    }

                    ofs << solution.first.back() << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value("--reference-pareto") + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                                         arg_parser.option_value("--reference-pareto") +
                                         " not created.");
            }
        }
    } else {
        std::cerr << "./reference_pareto_front_calculator_exec "
                  << "--instance <instance_filename> "
                  << "--pareto-i <pareto_filename> "
                  << "--best-solutions-snapshots-i <best_solutions_snapshots_filename> "
                  << "--reference-pareto <reference_pareto_filename> "
                  << std::endl;
    }

    return 0;
}
