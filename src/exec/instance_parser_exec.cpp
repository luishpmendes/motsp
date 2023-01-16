#include "utils/argument_parser.hpp"
#include "instance/instance.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);
    unsigned num_objectives, i, aux, x, y;
    std::vector<std::vector<std::pair<double, double>>> coord;
    std::ifstream ifs;
    std::string line;
    std::stringstream ss;
    motsp::Instance instance;

    for(num_objectives = 0;
        arg_parser.option_exists("--instance-" +
                                 std::to_string(num_objectives));
        num_objectives++) {}
    
    coord.resize(num_objectives);

    for (i = 0; i < num_objectives; i++) {
        ifs.open(arg_parser.option_value("--instance-" +
                                         std::to_string(i)));
        if (ifs.is_open()) {
            // Ignore first 6 lines
            std::getline(ifs, line);
            std::getline(ifs, line);
            std::getline(ifs, line);
            std::getline(ifs, line);
            std::getline(ifs, line);
            std::getline(ifs, line);

            while (std::getline(ifs, line)) {
                ss.clear();
                ss.str(line);

                if (ss >> aux >> x >> y) {
                    coord[i].push_back(std::make_pair(x, y));
                } else {
                    break;
                }
            }

            ifs.close();
        } else {
            throw("Error: could not open file " +
                  arg_parser.option_value("--instance-" +
                                          std::to_string(i)));
        }
    }

    instance = motsp::Instance(coord);

    std::cout << instance;

    return 0;
}
