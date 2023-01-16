#include "instance/instance.hpp"
#include "utils/argument_parser.hpp"
#include <pagmo/utils/hypervolume.hpp>
#include <fstream>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);

    if(arg_parser.option_exists("--instance") &&
       arg_parser.option_exists("--reference-pareto")) {
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

        std::vector<std::vector<double>> reference_pareto;
        std::vector<std::vector<std::vector<double>>> paretos;
        std::vector<std::vector<unsigned>> iteration_snapshots;
        std::vector<std::vector<double>> time_snapshots;
        std::vector<std::vector<std::vector<std::vector<double>>>>
            best_solutions_snapshots;
        unsigned num_solvers;
        std::vector<double> min_cost(instance.num_objectives,
                                      std::numeric_limits<double>::max()),
                            max_cost = instance.primal_bound,
                            reference_point(instance.num_objectives, 1.1);
        double max_hypervolume = 1.0;

        ifs.open(arg_parser.option_value("--reference-pareto"));

        if(ifs.is_open()) {
            for(std::string line; std::getline(ifs, line);) {
                std::istringstream iss(line);
                std::vector<double> cost(instance.num_objectives, 0.0);

                for(unsigned j = 0; j < instance.num_objectives; j++) {
                    iss >> cost[j];

                    if(min_cost[j] > cost[j]) {
                        min_cost[j] = cost[j];
                    }

                    assert(cost[j] <= max_cost[j]);
                }

                reference_pareto.push_back(cost);
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--reference-pareto") +
                    " not found.");
        }

        for (const double & p : reference_point) {
            max_hypervolume *= p;
        }

        assert(max_hypervolume > 1.1);

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

        paretos.resize(num_solvers);
        iteration_snapshots.resize(num_solvers);
        time_snapshots.resize(num_solvers);
        best_solutions_snapshots.resize(num_solvers);

        for(unsigned i = 0; i < num_solvers; i++) {
            if(arg_parser.option_exists("--pareto-" + std::to_string(i))) {
                ifs.open(arg_parser.option_value("--pareto-" +
                                                 std::to_string(i)));

                if(ifs.is_open()) {
                    for(std::string line; std::getline(ifs, line);) {
                        std::istringstream iss(line);
                        std::vector<double> cost(instance.num_objectives, 0.0);

                        for(unsigned j = 0; j < instance.num_objectives; j++) {
                            iss >> cost[j];

                            if(min_cost[j] > cost[j]) {
                                min_cost[j] = cost[j];
                            }

                            assert(cost[j] <= max_cost[j]);
                        }

                        paretos[i].push_back(cost);
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

                        iteration_snapshots[i].push_back(iteration);
                        time_snapshots[i].push_back(time);
                        best_solutions_snapshots[i].emplace_back();

                        ifs.ignore();

                        for(std::string line; std::getline(ifs, line);) {
                            std::istringstream iss(line);
                            std::vector<double> cost(instance.num_objectives,
                                                     0.0);

                            for(unsigned j = 0;
                                j < instance.num_objectives;
                                j++) {
                                iss >> cost[j];

                                if(min_cost[j] > cost[j]) {
                                    min_cost[j] = cost[j];
                                }

                                assert(cost[j] <= max_cost[j]);
                            }

                            best_solutions_snapshots[i].back().push_back(cost);
                        }

                        ifs.close();
                    } else {
                        break;
                    }
                }
            }
        }

        std::vector<std::vector<double>> normalized_pareto(
                        reference_pareto.size());

        for(unsigned i = 0; i < reference_pareto.size(); i++) {
            normalized_pareto[i] = std::vector<double>(
                    reference_pareto[i].size(), 0.0);

            for(unsigned j = 0; j < instance.num_objectives; j++) {
                normalized_pareto[i][j] =
                    (reference_pareto[i][j] - min_cost[j]) /
                    (max_cost[j] - min_cost[j]);
                assert(normalized_pareto[i][j] >= 0.0);
                assert(normalized_pareto[i][j] <= 1.0);
            }
        }

        pagmo::hypervolume hv(normalized_pareto);

        double reference_hypervolume = hv.compute(reference_point);
        assert(reference_hypervolume >= 0.0);
        assert(reference_hypervolume <= max_hypervolume);

        double normalized_reference_hypervolume = reference_hypervolume
                                                / max_hypervolume;
        assert(normalized_reference_hypervolume >= 0.0);
        assert(normalized_reference_hypervolume <= 1.0);

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;

            ofs.open(arg_parser.option_value("--hypervolume-" +
                                             std::to_string(i)));

            if(ofs.is_open()) {
                normalized_pareto.resize(paretos[i].size());

                for(unsigned j = 0; j < paretos[i].size(); j++) {
                    normalized_pareto[j] = std::vector<double>(
                            paretos[i][j].size(), 0.0);

                    for(unsigned k = 0; k < instance.num_objectives; k++) {
                        normalized_pareto[j][k] =
                            (paretos[i][j][k] - min_cost[k]) /
                            (max_cost[k] - min_cost[k]);
                        assert(normalized_pareto[j][k] >= 0.0);
                        assert(normalized_pareto[j][k] <= 1.0);
                    }
                }

                hv = pagmo::hypervolume(normalized_pareto);

                double hypervolume = hv.compute(reference_point);
                assert(hypervolume >= 0.0);
                assert(hypervolume <= max_hypervolume);

                double normalized_hypervolume = hypervolume / max_hypervolume;
                assert(normalized_hypervolume >= 0.0);
                assert(normalized_hypervolume <= 1.0);

                double hypervolume_ratio = normalized_hypervolume /
                                           normalized_reference_hypervolume;
                assert(hypervolume_ratio >= 0.0);
                assert(hypervolume_ratio <= 1.0);

                ofs << hypervolume_ratio << std::endl;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--hypervolume-" +
                                std::to_string(i)) + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-" +
                            std::to_string(i)) + " not created.");
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;

            ofs.open(arg_parser.option_value("--hypervolume-snapshots-" +
                                             std::to_string(i)));

            if(ofs.is_open()) {
                for(unsigned j = 0;
                    j < best_solutions_snapshots[i].size();
                    j++) {
                    std::vector<std::vector<double>>
                        normalized_pareto_snapshot(
                                best_solutions_snapshots[i][j].size());

                    for(unsigned k = 0;
                        k < best_solutions_snapshots[i][j].size();
                        k++) {
                        normalized_pareto_snapshot[k] = std::vector<double>(
                                best_solutions_snapshots[i][j][k].size(), 0.0);

                        for(unsigned l = 0; l < instance.num_objectives; l++) {
                            normalized_pareto_snapshot[k][l] =
                                (best_solutions_snapshots[i][j][k][l] -
                                 min_cost[l]) / (max_cost[l] - min_cost[l]);
                            assert(normalized_pareto_snapshot[k][l] >= 0.0);
                            assert(normalized_pareto_snapshot[k][l] <= 1.0);
                        }
                    }

                    hv = pagmo::hypervolume(normalized_pareto_snapshot);

                    double hypervolume = hv.compute(reference_point);
                    assert(hypervolume >= 0.0);
                    assert(hypervolume <= max_hypervolume);

                    double normalized_hypervolume = hypervolume /
                        max_hypervolume;
                    assert(normalized_hypervolume >= 0.0);
                    assert(normalized_hypervolume <= 1.0);

                    double hypervolume_ratio = normalized_hypervolume /
                        normalized_reference_hypervolume;
                    assert(hypervolume_ratio >= 0.0);
                    assert(hypervolume_ratio <= 1.0);

                    ofs << iteration_snapshots[i][j] << ","
                        << time_snapshots[i][j] << ","
                        << hypervolume_ratio << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--hypervolume-snapshots-" +
                                    std::to_string(i)) + ".");
                    }
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
                  << "--reference-pareto <reference_pareto_filename> "
                  << "--pareto-i <pareto_filename> "
                  << "--best-solutions-snapshots-i <best_solutions_snapshots_filename> "
                  << "--hypervolume-i <hypervolume_filename> "
                  << "--hypervolume-snapshots-i <hypervolume_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}
