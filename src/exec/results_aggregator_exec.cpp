#include "utils/argument_parser.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <numeric>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);
    std::vector<double> hypervolume_values, igd_plus_values, multiplicative_epsilon_values;

    unsigned num_hypervolumes, index_best, index_median, num_igd_pluses, num_multiplicative_epsilons;
    std::vector<std::pair<double, unsigned>> hypervolumes, igd_pluses, multiplicative_epsilons;

    for(num_hypervolumes = 0;
        arg_parser.option_exists("--hypervolume-" +
                                 std::to_string(num_hypervolumes));
        num_hypervolumes++) {}

    for(num_igd_pluses = 0;
        arg_parser.option_exists("--igd-plus-" +
                                 std::to_string(num_igd_pluses));
        num_igd_pluses++) {}
    
    for(num_multiplicative_epsilons = 0;
        arg_parser.option_exists("--igd-plus-" +
                                 std::to_string(num_multiplicative_epsilons));
        num_multiplicative_epsilons++) {}

    hypervolumes.resize(num_hypervolumes);
    igd_pluses.resize(num_igd_pluses);
    multiplicative_epsilons.resize(num_multiplicative_epsilons);

    for(unsigned i = 0; i < num_hypervolumes; i++) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--hypervolume-" + std::to_string(i)));

        if(ifs.is_open()) {
            ifs >> hypervolumes[i].first;

            if(ifs.eof() || ifs.fail() || ifs.bad()) {
                throw std::runtime_error("Error reading file " +
                        arg_parser.option_value("--hypervolume-" +
                            std::to_string(i)) + ".");
            }

            hypervolumes[i].second = i;
            ifs.close();
            hypervolume_values.push_back(hypervolumes[i].first);
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolume-" +
                        std::to_string(i)) + " not found.");
        }
    }

    for(unsigned i = 0; i < num_igd_pluses; i++) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--igd-plus-" + std::to_string(i)));

        if(ifs.is_open()) {
            ifs >> igd_pluses[i].first;

            if(ifs.eof() || ifs.fail() || ifs.bad()) {
                throw std::runtime_error("Error reading file " +
                        arg_parser.option_value("--igd-plus-" +
                            std::to_string(i)) + ".");
            }

            igd_pluses[i].second = i;
            ifs.close();
            igd_plus_values.push_back(igd_pluses[i].first);
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--igd-plus-" +
                        std::to_string(i)) + " not found.");
        }
    }

    for(unsigned i = 0; i < num_multiplicative_epsilons; i++) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--multiplicative-epsilon-" + std::to_string(i)));

        if(ifs.is_open()) {
            ifs >> multiplicative_epsilons[i].first;

            if(ifs.eof() || ifs.fail() || ifs.bad()) {
                throw std::runtime_error("Error reading file " +
                        arg_parser.option_value("--multiplicative-epsilon-" +
                            std::to_string(i)) + ".");
            }

            multiplicative_epsilons[i].second = i;
            ifs.close();
            multiplicative_epsilon_values.push_back(multiplicative_epsilons[i].first);
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--multiplicative-epsilon-" +
                        std::to_string(i)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--hypervolumes")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolumes"));

        if(ofs.is_open()) {
            for(const std::pair<double, unsigned> & hv : hypervolumes) {
                ofs << hv.first << std::endl;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--hypervolumes") + ".");
                }
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolumes") +
                    " not created.");
        }
    }

    if(arg_parser.option_exists("--igd-pluses")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--igd-pluses"));

        if(ofs.is_open()) {
            for(const std::pair<double, unsigned> & igd_plus : igd_pluses) {
                ofs << igd_plus.first << std::endl;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--igd-pluses") + ".");
                }
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--igd-pluses") +
                    " not created.");
        }
    }

    if(arg_parser.option_exists("--multiplicative-epsilons")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--multiplicative-epsilons"));

        if(ofs.is_open()) {
            for(const std::pair<double, unsigned> & multiplicative_epsilon : multiplicative_epsilons) {
                ofs << multiplicative_epsilon.first << std::endl;

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--multiplicative-epsilons") + ".");
                }
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--multiplicative_epsilons") +
                    " not created.");
        }
    }

    if(arg_parser.option_exists("--hypervolume-statistics")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolume-statistics"));

        if(ofs.is_open()) {
            double hypervolume_mean = std::accumulate(hypervolume_values.begin(), hypervolume_values.end(), 0.0) / hypervolume_values.size();
            double hypervolume_var = std::accumulate(hypervolume_values.begin(), hypervolume_values.end(), 0.0, [hypervolume_mean](double acc, double val) {
                return acc + (val - hypervolume_mean) * (val - hypervolume_mean);
            }) / hypervolume_values.size();
            double hypervolume_std = std::sqrt(hypervolume_var);

            ofs << hypervolume_mean << ", " << hypervolume_std << std::endl;

            if(ofs.eof() || ofs.fail() || ofs.bad()) {
                throw std::runtime_error("Error writing file " +
                        arg_parser.option_value("--hypervolume-statistics") +
                        ".");
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolume-statistics") +
                    " not created.");
        }
    }

    if(arg_parser.option_exists("--igd-plus-statistics")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--igd-plus-statistics"));

        if(ofs.is_open()) {
            double igd_plus_mean = std::accumulate(igd_plus_values.begin(), igd_plus_values.end(), 0.0) / igd_plus_values.size();
            double igd_plus_var = std::accumulate(igd_plus_values.begin(), igd_plus_values.end(), 0.0, [igd_plus_mean](double acc, double val) {
                return acc + (val - igd_plus_mean) * (val - igd_plus_mean);
            }) / igd_plus_values.size();
            double igd_plus_std = std::sqrt(igd_plus_var);

            ofs << igd_plus_mean << ", " << igd_plus_std << std::endl;

            if(ofs.eof() || ofs.fail() || ofs.bad()) {
                throw std::runtime_error("Error writing file " +
                        arg_parser.option_value("--igd-plus-statistics") +
                        ".");
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--igd-plus-statistics") +
                    " not created.");
        }
    }

    if(arg_parser.option_exists("--multiplicative-epsilon-statistics")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--multiplicative-epsilon-statistics"));

        if(ofs.is_open()) {
            double multiplicative_epsilon_mean = std::accumulate(multiplicative_epsilon_values.begin(), multiplicative_epsilon_values.end(), 0.0) / multiplicative_epsilon_values.size();
            double multiplicative_epsilon_var = std::accumulate(multiplicative_epsilon_values.begin(), multiplicative_epsilon_values.end(), 0.0, [multiplicative_epsilon_mean](double acc, double val) {
                return acc + (val - multiplicative_epsilon_mean) * (val - multiplicative_epsilon_mean);
            }) / multiplicative_epsilon_values.size();
            double multiplicative_epsilon_std = std::sqrt(multiplicative_epsilon_var);

            ofs << multiplicative_epsilon_mean << ", " << multiplicative_epsilon_std << std::endl;

            if(ofs.eof() || ofs.fail() || ofs.bad()) {
                throw std::runtime_error("Error writing file " +
                        arg_parser.option_value("--multiplicative-epsilon-statistics") +
                        ".");
            }

            ofs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--multiplicative-epsilon-statistics") +
                    " not created.");
        }
    }

    std::sort(hypervolumes.begin(), hypervolumes.end());
    index_best = hypervolumes.back().second;
    index_median = hypervolumes[hypervolumes.size() / 2].second;

    if(arg_parser.option_exists("--statistics-best") &&
       arg_parser.option_exists("--statistics-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--statistics-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--statistics-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--statistics-best") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--statistics-best") +
                        " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--statistics-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--statistics-median") &&
       arg_parser.option_exists("--statistics-" +
                                std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--statistics-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--statistics-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--statistics-median") +
                            ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--statistics-median") +
                        " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--statistics-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--pareto-best") &&
       arg_parser.option_exists("--pareto-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--pareto-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--pareto-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--pareto-best") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--pareto-best") +
                        " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--pareto-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--pareto-median") &&
       arg_parser.option_exists("--pareto-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--pareto-" +
                                         std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--pareto-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value("--pareto-median") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--pareto-median") +
                        " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--pareto-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--hypervolume-snapshots-best") &&
       arg_parser.option_exists("--hypervolume-snapshots-" +
                                std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--hypervolume-snapshots-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--hypervolume-snapshots-best")
                            + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--hypervolume-snapshots-best")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolume-snapshots-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--hypervolume-snapshots-median") &&
       arg_parser.option_exists("--hypervolume-snapshots-" +
                                std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--hypervolume-snapshots-" +
                                         std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--hypervolume-snapshots-median")
                            + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value(
                            "--hypervolume-snapshots-median")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolume-snapshots-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--best-solutions-snapshots-best") &&
       arg_parser.option_exists("--best-solutions-snapshots-" +
                                std::to_string(index_best))) {
        std::string best_solutions_snapshots_best_filename =
            arg_parser.option_value("--best-solutions-snapshots-best");
        std::string best_solutions_snapshots_index_best_filename =
            arg_parser.option_value("--best-solutions-snapshots-" +
                                    std::to_string(index_best));

        for(unsigned i = 0; ; i++) {
            std::ifstream ifs;
            ifs.open(best_solutions_snapshots_index_best_filename +
                     std::to_string(i) + ".txt",
                     std::ios::binary);

            if(ifs.is_open()) {
                std::ofstream ofs;
                ofs.open(best_solutions_snapshots_best_filename +
                         std::to_string(i) + ".txt",
                         std::ios::binary);

                if(ofs.is_open()) {
                    ofs << ifs.rdbuf();

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                best_solutions_snapshots_best_filename +
                                std::to_string(i) + ".txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                            best_solutions_snapshots_best_filename +
                            std::to_string(i) + ".txt not created.");
                }

                ifs.close();
            } else {
                break;
            }
        }
    }

    if(arg_parser.option_exists("--best-solutions-snapshots-median") &&
       arg_parser.option_exists("--best-solutions-snapshots-" +
                                std::to_string(index_median))) {
        std::string best_solutions_snapshots_median_filename =
            arg_parser.option_value("--best-solutions-snapshots-median");
        std::string best_solutions_snapshots_index_median_filename =
            arg_parser.option_value("--best-solutions-snapshots-" +
                                    std::to_string(index_median));

        for(unsigned i = 0; ; i++) {
            std::ifstream ifs;
            ifs.open(best_solutions_snapshots_index_median_filename +
                     std::to_string(i) + ".txt",
                     std::ios::binary);

            if(ifs.is_open()) {
                std::ofstream ofs;
                ofs.open(best_solutions_snapshots_median_filename +
                         std::to_string(i) + ".txt",
                         std::ios::binary);

                if(ofs.is_open()) {
                    ofs << ifs.rdbuf();

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                best_solutions_snapshots_median_filename +
                                std::to_string(i) + ".txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                            best_solutions_snapshots_median_filename +
                            std::to_string(i) + ".txt not created.");
                }

                ifs.close();
            } else {
                break;
            }
        }
    }

    if(arg_parser.option_exists("--num-non-dominated-snapshots-best") &&
       arg_parser.option_exists("--num-non-dominated-snapshots-" +
                                std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-non-dominated-snapshots-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value(
                        "--num-non-dominated-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-non-dominated-snapshots-best")
                            + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value(
                            "--num-non-dominated-snapshots-best")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-non-dominated-snapshots-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-non-dominated-snapshots-median") &&
       arg_parser.option_exists("--num-non-dominated-snapshots-" +
                                std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-non-dominated-snapshots-" +
                                         std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value(
                        "--num-non-dominated-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-non-dominated-snapshots-median")
                            + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value(
                            "--num-non-dominated-snapshots-median")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-non-dominated-snapshots-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-fronts-snapshots-best") &&
       arg_parser.option_exists("--num-fronts-snapshots-" +
                                std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-fronts-snapshots-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-fronts-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-non-dominated-snapshots-best") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--num-fronts-snapshots-best")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-fronts-snapshots-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-fronts-snapshots-median") &&
       arg_parser.option_exists(
           "--num-fronts-snapshots-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--num-fronts-snapshots-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-fronts-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-fronts-snapshots-median") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--num-fronts-snapshots-median")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-fronts-snapshots-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--populations-snapshots-best") &&
       arg_parser.option_exists("--populations-snapshots-" +
                                std::to_string(index_best))) {
        std::string populations_snapshots_best_filename =
            arg_parser.option_value("--populations-snapshots-best");
        std::string populations_snapshots_index_best_filename =
            arg_parser.option_value("--populations-snapshots-" +
                                    std::to_string(index_best));

        for(unsigned i = 0; ; i++) {
            std::ifstream ifs;
            ifs.open(populations_snapshots_index_best_filename +
                     std::to_string(i) + ".txt",
                     std::ios::binary);

            if(ifs.is_open()) {
                std::ofstream ofs;
                ofs.open(populations_snapshots_best_filename +
                         std::to_string(i) + ".txt",
                         std::ios::binary);

                if(ofs.is_open()) {
                    ofs << ifs.rdbuf();

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                populations_snapshots_best_filename +
                                std::to_string(i) + ".txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                            populations_snapshots_best_filename +
                            std::to_string(i) + ".txt not created.");
                }

                ifs.close();
            } else {
                break;
            }
        }
    }

    if(arg_parser.option_exists("--populations-snapshots-median") &&
       arg_parser.option_exists("--populations-snapshots-" +
                                std::to_string(index_median))) {
        std::string populations_snapshots_median_filename =
            arg_parser.option_value("--populations-snapshots-median");
        std::string populations_snapshots_index_median_filename =
            arg_parser.option_value("--populations-snapshots-" +
                                    std::to_string(index_median));

        for(unsigned i = 0; ; i++) {
            std::ifstream ifs;
            ifs.open(populations_snapshots_index_median_filename +
                     std::to_string(i) + ".txt",
                     std::ios::binary);

            if(ifs.is_open()) {
                std::ofstream ofs;
                ofs.open(populations_snapshots_median_filename +
                         std::to_string(i) + ".txt",
                         std::ios::binary);

                if(ofs.is_open()) {
                    ofs << ifs.rdbuf();

                    if(ofs.eof() || ofs.fail() || ofs.bad()) {
                        throw std::runtime_error("Error writing file " +
                                populations_snapshots_median_filename +
                                std::to_string(i) + ".txt.");
                    }

                    ofs.close();
                } else {
                    throw std::runtime_error("File " +
                            populations_snapshots_median_filename +
                            std::to_string(i) + ".txt not created.");
                }

                ifs.close();
            } else {
                break;
            }
        }
    }

    if(arg_parser.option_exists("--num-elites-snapshots-best") &&
       arg_parser.option_exists("--num-elites-snapshots-" +
                                std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-elites-snapshots-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-elites-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-elites-snapshots-best") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--num-elites-snapshots-best")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-elites-snapshots-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-elites-snapshots-median") &&
       arg_parser.option_exists("--num-elites-snapshots-" +
                                std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-elites-snapshots-" +
                                         std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-elites-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-elites-snapshots-median") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--num-elites-snapshots-median")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-elites-snapshots-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-mutants-snapshots-best") &&
       arg_parser.option_exists("--num-mutants-snapshots-" +
                                std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-mutants-snapshots-" +
                                         std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-mutants-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();

                if(ofs.eof() || ofs.fail() || ofs.bad()) {
                    throw std::runtime_error("Error writing file " +
                            arg_parser.option_value(
                                "--num-mutants-snapshots-best") + ".");
                }

                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value("--num-mutants-snapshots-best")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-mutants-snapshots-" +
                        std::to_string(index_best)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--num-mutants-snapshots-median") &&
       arg_parser.option_exists("--num-mutants-snapshots-" +
                                std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--num-mutants-snapshots-" +
                                         std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--num-mutants-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw std::runtime_error("File " +
                        arg_parser.option_value(
                            "--num-mutants-snapshots-median")
                        + " not created.");
            }

            ifs.close();
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--num-mutants-snapshots-" +
                        std::to_string(index_median)) + " not found.");
        }
    }

    return 0;
}
