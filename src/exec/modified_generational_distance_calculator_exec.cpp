#include "instance/instance.hpp"
#include "utils/argument_parser.hpp"
#include <cassert>
#include <fstream>

static inline
double modified_distance(const std::vector<NSBRKGA::Sense> & senses,
                         const std::vector<double> & reference_point,
                         const std::vector<double> & point) {
    double distance = 0.0, delta;

    for(unsigned i = 0; i < senses.size(); i++) {
        delta = 0;

        if(senses[i] == NSBRKGA::Sense::MINIMIZE) {
            if(point[i] > reference_point[i]) {
                delta = point[i] - reference_point[i];
            }
        } else { // senses[i] == NSBRKGA::Sense::MAXIMIZE
            if(reference_point[i] > point[i]) {
                delta = reference_point[i] - point[i];
            }
        }

        distance += delta * delta;
    }

    return distance;
}

static inline
double modified_inverted_generational_distance(
        const std::vector<NSBRKGA::Sense> & senses,
        const std::vector<std::vector<double>> & reference_front,
        const std::vector<std::vector<double>> & front) {
    double igd_plus = 0.0, min_distance, distance;

    for(unsigned i = 0; i < reference_front.size(); i++) {
        min_distance = modified_distance(senses,
                                         reference_front[i],
                                         front.front());

        for(unsigned j = 1; j < front.size(); j++) {
            distance = modified_distance(senses, reference_front[i], front[j]);

            if(distance < min_distance) {
                min_distance = distance;
            }
        }

        igd_plus += min_distance;
    }

    return igd_plus / reference_front.size();
}

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
                            max_cost = instance.primal_bound;
        std::vector<double> igd_pluses;
        std::vector<std::vector<double>> igd_plus_snapshots;
        double max_igd_plus = 0.0;

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

        for(num_solvers = 0;
            arg_parser.option_exists("--pareto-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--best-solutions-snapshots-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--igd-plus-" +
                                     std::to_string(num_solvers)) ||
            arg_parser.option_exists("--igd-plus-snapshots-" +
                                     std::to_string(num_solvers));
            num_solvers++) {}

        paretos.resize(num_solvers);
        iteration_snapshots.resize(num_solvers);
        time_snapshots.resize(num_solvers);
        best_solutions_snapshots.resize(num_solvers);
        igd_pluses.resize(num_solvers);
        igd_plus_snapshots.resize(num_solvers, std::vector<double>());
        igd_plus_snapshots.assign(num_solvers, std::vector<double>());

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

        std::vector<std::vector<double>> normalized_reference_pareto(
                        reference_pareto.size());

        for(unsigned i = 0; i < reference_pareto.size(); i++) {
            normalized_reference_pareto[i] = std::vector<double>(
                    reference_pareto[i].size(), 0.0);

            for(unsigned j = 0; j < instance.num_objectives; j++) {
                normalized_reference_pareto[i][j] =
                    (reference_pareto[i][j] - min_cost[j]) /
                    (max_cost[j] - min_cost[j]);
                assert(normalized_reference_pareto[i][j] >= 0.0);
                assert(normalized_reference_pareto[i][j] <= 1.0);
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            std::vector<std::vector<double>> normalized_pareto(
                paretos[i].size());

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

            double igd_plus = modified_inverted_generational_distance(
                    instance.senses,
                    normalized_reference_pareto,
                    normalized_pareto);

            assert(igd_plus >= 0.0);

            igd_pluses[i] = igd_plus;

            if (max_igd_plus < igd_plus) {
                max_igd_plus = igd_plus;
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
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

                double igd_plus = modified_inverted_generational_distance(
                        instance.senses,
                        normalized_reference_pareto,
                        normalized_pareto_snapshot);

                assert(igd_plus >= 0.0);

                igd_plus_snapshots[i].push_back(igd_plus);

                if (max_igd_plus < igd_plus) {
                    max_igd_plus = igd_plus;
                }
            }
        }

        std::cout << "max_igd_plus : " << max_igd_plus << std::endl;

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--igd-plus-" +
                                             std::to_string(i)));

            if(ofs.is_open()) {
                double normalized_igd_plus = igd_pluses[i] / max_igd_plus;

                assert(normalized_igd_plus >= 0.0);
                assert(normalized_igd_plus <= 1.0);

                ofs << normalized_igd_plus << std::endl;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--igd-plus-" +
                                std::to_string(i)) + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--igd-plus-" +
                            std::to_string(i)) + " not created.");
            }
        }

        for(unsigned i = 0; i < num_solvers; i++) {
            std::ofstream ofs;

            ofs.open(arg_parser.option_value("--igd-plus-snapshots-" +
                                             std::to_string(i)));

            if(ofs.is_open()) {
                for(unsigned j = 0;
                    j < best_solutions_snapshots[i].size();
                    j++) {
                    double normalized_igd_plus = igd_plus_snapshots[i][j] /
                        max_igd_plus;

                    assert(normalized_igd_plus >= 0.0);
                    assert(normalized_igd_plus <= 1.0);

                    ofs << iteration_snapshots[i][j] << ","
                        << time_snapshots[i][j] << ","
                        << normalized_igd_plus << std::endl;

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                arg_parser.option_value(
                                    "--igd-plus-snapshots-" +
                                    std::to_string(i)) + ".");
                    }
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--igd-plus-snapshots-" +
                            std::to_string(i)) + " not created.");
            }
        }
    } else {
        std::cerr << "./modified_inverted_generational_distance_exec "
                  << "--instance <instance_filename> "
                  << "--reference-pareto <reference_pareto_filename> "
                  << "--pareto-i <pareto_filename> "
                  << "--best-solutions-snapshots-i <best_solutions_snapshots_filename> "
                  << "--igd-plus-i <modified_inverted_generational_distance_filename> "
                  << "--igd-plus-snapshots-i <modified_inverted_generational_distance_snapshots_filename> "
                  << std::endl;
    }

    return 0;
}
