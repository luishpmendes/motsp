#include "utils/argument_parser.hpp"
#include "utils/statistics.hpp"
#include <algorithm>
#include <fstream>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);
    Statistics stats;

    unsigned num_hypervolumes, index_best, index_median;
    std::vector<std::pair<double, unsigned>> hypervolumes;

    for(num_hypervolumes = 0;
        arg_parser.option_exists("--hypervolume-" +
                                 std::to_string(num_hypervolumes));
        num_hypervolumes++) {}

    hypervolumes.resize(num_hypervolumes);

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
            stats.insert(hypervolumes[i].first);
        } else {
            throw std::runtime_error("File " +
                    arg_parser.option_value("--hypervolume-" +
                        std::to_string(i)) + " not found.");
        }
    }

    if(arg_parser.option_exists("--hypervolumes")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolumes"));

        if(ofs.is_open()) {
            for(const auto hv : hypervolumes) {
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

    std::sort(hypervolumes.begin(), hypervolumes.end());
    index_best = hypervolumes.back().second;
    index_median = hypervolumes[hypervolumes.size() / 2].second;

    if(arg_parser.option_exists("--hypervolume-statistics")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolume-statistics"));

        if(ofs.is_open()) {
            ofs << stats.mean << ", " << stats.standard_deviation << std::endl;

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

