#include "instance/instance.hpp"
#include "utils/argument_parser.hpp"
#include <pagmo/utils/hypervolume.hpp>
#include <fstream>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if(arg_parser.option_exists("--instance")) {
        std::ifstream ifs;
        motsp::Instance instance;
        ifs.open(arg_parser.option_value("--instance"));
        ifs >> instance;
        ifs.close();

        std::vector<std::vector<std::vector<double>>> paretos;
        std::vector<std::vector<unsigned>> iteration_snapshots;
        std::vector<std::vector<double>> time_snapshots;
        std::vector<std::vector<std::vector<std::vector<double>>>>
            pareto_snapshots;
        std::vector<double> min_costs(instance.num_objectives,
                                      std::numeric_limits<double>::max()),
                            max_costs(instance.num_objectives,
                                      std::numeric_limits<double>::lowest());
        std::vector<double> reference_point(instance.num_objectives, 1.1);
        unsigned num_solvers;

        for(num_solvers = 0;
            arg_parser.option_exists("--pareto-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--pareto-snapshots-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--hypervolume-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--hypervolume-snapshots-" +
                                     std::to_string(num_solvers));
            num_solvers++) {}

        paretos.resize(num_solvers);
        iteration_snapshots.resize(num_solvers);
        time_snapshots.resize(num_solvers);
        pareto_snapshots.resize(num_solvers);

        for(unsigned i = 0; i < num_solvers; i++) {
            if(arg_parser.option_exists("--pareto-" + std::to_string(i))) {
                ifs.open(arg_parser.option_value(
                            "--pareto-" + std::to_string(i)));

                if(ifs.is_open()) {
                    for(std::string line; std::getline(ifs, line);) {
                        std::istringstream iss(line);
                        std::vector<double> costs(instance.num_objectives, 0.0);

                        for(unsigned j = 0; j < instance.num_objectives; j++) {
                            if(!(iss >> costs[j])) {
                                throw std::runtime_error("Error reading file "
                                        + arg_parser.option_value("--pareto-" +
                                            std::to_string(i)) + ".");
                            }

                            if(min_costs[j] > costs[j]) {
                                min_costs[j] = costs[j];
                            }

                            if(max_costs[j] < costs[j]) {
                                max_costs[j] = costs[j];
                            }
                        }

                        paretos[i].push_back(costs);
                    }

                    ifs.close();
                } else {
                    throw std::runtime_error("File " +
                            arg_parser.option_value("--pareto-" +
                                std::to_string(i)) + " not found.");
                }
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            if(arg_parser.option_exists(
                        "--pareto-snapshots-" + std::to_string(i))) {
                std::string pareto_snapshots_filename =
                    arg_parser.option_value(
                            "--pareto-snapshots-" + std::to_string(i));

                for(unsigned j = 0; ; j++) {
                    ifs.open(pareto_snapshots_filename + std::to_string(j) +
                            ".txt");

                    if(ifs.is_open()) {
                        unsigned iteration;
                        double time;

                        ifs >> iteration >> time;

                        iteration_snapshots[i].push_back(iteration);
                        time_snapshots[i].push_back(time);
                        pareto_snapshots[i].emplace_back();

                        ifs.ignore();

                        for(std::string line; std::getline(ifs, line);) {
                            std::istringstream iss(line);
                            std::vector<double> costs(instance.num_objectives,
                                                      0.0);

                            for(unsigned j = 0;
                                j < instance.num_objectives;
                                j++) {
                                if(!(iss >> costs[j])) {
                                    throw std::runtime_error(
                                            "Error reading file" +
                                            pareto_snapshots_filename +
                                            std::to_string(j) + ".txt" + ".");
                                }

                                if(min_costs[j] > costs[j]) {
                                    min_costs[j] = costs[j];
                                }

                                if(max_costs[j] < costs[j]) {
                                    max_costs[j] = costs[j];
                                }
                            }

                            pareto_snapshots[i].back().push_back(costs);
                        }

                        ifs.close();
                    } else {
                        break;
                    }
                }
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value(
                        "--hypervolume-" + std::to_string(i)));

            if(ofs.is_open()) {
                std::vector<std::vector<double>> normalized_pareto(
                        paretos[i].size());
                for(unsigned j = 0; j < paretos[i].size(); j++) {
                    normalized_pareto[j] = std::vector<double>(
                            paretos[i][j].size(), 0.0);
                    for(unsigned k = 0; k < instance.num_objectives; k++) {
                        normalized_pareto[j][k] =
                            (paretos[i][j][k] - min_costs[k]) /
                            (max_costs[k] - min_costs[k]);
                    }
                }

                pagmo::hypervolume hv(normalized_pareto);
                double hypervolume = hv.compute(reference_point) / (1.1 * 1.1);
                ofs << hypervolume << std::endl;
                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-" +
                            std::to_string(i)) + " not created.");
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value(
                        "--hypervolume-snapshots-" + std::to_string(i)));

            if(ofs.is_open()) {
                for(unsigned j = 0;
                    j < pareto_snapshots[i].size();
                    j++) {
                    std::vector<std::vector<double>>
                        normalized_pareto_snapshot(
                                pareto_snapshots[i][j].size());
                    for(unsigned k = 0; k < pareto_snapshots[i][j].size(); k++) {
                        normalized_pareto_snapshot[k] = std::vector<double>(
                                pareto_snapshots[i][j][k].size(), 0.0);
                        for(unsigned l = 0; l < instance.num_objectives; l++) {
                            normalized_pareto_snapshot[k][l] =
                                (pareto_snapshots[i][j][k][l] - min_costs[l]) /
                                (max_costs[l] - min_costs[l]);
                        }
                    }

                    pagmo::hypervolume hv(normalized_pareto_snapshot);
                    double hypervolume = hv.compute(reference_point) /
                        (1.1 * 1.1);
                    ofs << iteration_snapshots[i][j] << ","
                        << time_snapshots[i][j] << ","
                        << hypervolume << std::endl;
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-snapshots-" +
                            std::to_string(i)) + " not created.");
            }
        }
    } else {
        std::cerr << "./hypervolume_calculator_exec "
                  << "--instance <instance_filename> "
                  << "--pareto-i <pareto_filename> "
                  << "--pareto-snapshots-i <pareto_snapshots_filename> "
                  << "--hypervolume-i <hypervolume_filename> "
                  << "--hypervolume-snapshots-i <hypervolume_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}

