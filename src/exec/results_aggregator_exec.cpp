#include "utils/argument_parser.hpp"
#include "utils/statistics.hpp"
#include <algorithm>
#include <fstream>

int main(int argc, char * argv[]) {
    ArgumentParser arg_parser(argc, argv);
    Statistics stats;

    unsigned num_hypervolumes, index_best, index_median;
    std::vector<std::pair<double, unsigned>> hypervolumes;
    std::string pareto_snapshots_filename, hypervolume_snapshots_filename;

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
            hypervolumes[i].second = i;
            ifs.close();
            stats.insert(hypervolumes[i].first);
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--hypervolumes")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolumes"));
        for(const auto hv : hypervolumes) {
            ofs << hv.first << std::endl;
        }
        ofs.close();
    }

    std::sort(hypervolumes.begin(), hypervolumes.end());
    index_best = hypervolumes.back().second;
    index_median = hypervolumes[hypervolumes.size() / 2].second;

    if(arg_parser.option_exists("--hypervolume-statistics")) {
        std::ofstream ofs;
        ofs.open(arg_parser.option_value("--hypervolume-statistics"));
        ofs << stats.mean << ", " << stats.standard_deviation << std::endl;
        ofs.close();
    }

    if(arg_parser.option_exists("--statistics-best") &&
       arg_parser.option_exists("--statistics-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--statistics-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--statistics-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--statistics-median") &&
       arg_parser.option_exists(
           "--statistics-" + std::to_string(index_median))) {
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
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--pareto-best") &&
       arg_parser.option_exists("--pareto-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--pareto-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--pareto-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--pareto-median") &&
       arg_parser.option_exists("--pareto-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--pareto-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--pareto-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--hypervolume-snapshots-best") &&
       arg_parser.option_exists(
           "--hypervolume-snapshots-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--hypervolume-snapshots-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--hypervolume-snapshots-median") &&
       arg_parser.option_exists(
           "--hypervolume-snapshots-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--hypervolume-snapshots-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--hypervolume-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--non-dominated-snapshots-best") &&
       arg_parser.option_exists(
           "--non-dominated-snapshots-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--non-dominated-snapshots-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--non-dominated-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--non-dominated-snapshots-median") &&
       arg_parser.option_exists(
           "--non-dominated-snapshots-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value("--non-dominated-snapshots-"
                    + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value(
                        "--non-dominated-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--fronts-snapshots-best") &&
       arg_parser.option_exists(
           "--fronts-snapshots-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--fronts-snapshots-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--fronts-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--fronts-snapshots-median") &&
       arg_parser.option_exists(
           "--fronts-snapshots-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--fronts-snapshots-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--fronts-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--elite-sizes-snapshots-best") &&
       arg_parser.option_exists(
           "--elite-sizes-snapshots-" + std::to_string(index_best))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--elite-sizes-snapshots-" + std::to_string(index_best)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--elite-sizes-snapshots-best"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    if(arg_parser.option_exists("--elite-sizes-snapshots-median") &&
       arg_parser.option_exists(
           "--elite-sizes-snapshots-" + std::to_string(index_median))) {
        std::ifstream ifs;
        ifs.open(arg_parser.option_value(
                    "--elite-sizes-snapshots-" + std::to_string(index_median)),
                 std::ios::binary);

        if(ifs.is_open()) {
            std::ofstream ofs;
            ofs.open(arg_parser.option_value("--elite-sizes-snapshots-median"),
                     std::ios::binary);

            if(ofs.is_open()) {
                ofs << ifs.rdbuf();
                ofs.close();
            } else {
                throw "File not created.";
            }

            ifs.close();
        } else {
            throw "File not found.";
        }
    }

    return 0;
}

