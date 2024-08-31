#include "utils/argument_parser.hpp"
#include <chrono>
#include <iostream>
#include <random>

int main(int argc, char * argv[]) {
    Argument_Parser arg_parser(argc, argv);
    unsigned n, max_x, max_y, seed, i, x, y;
    std::string id;

    if (arg_parser.option_exists("--id") &&
        arg_parser.option_exists("--n") &&
        arg_parser.option_exists("--max-x") &&
        arg_parser.option_exists("--max-y")){
        n = std::stoul(arg_parser.option_value("--n"));
        max_x = std::stoul(arg_parser.option_value("--max-x"));
        max_y = std::stoul(arg_parser.option_value("--max-y"));
        id = arg_parser.option_value("--id");

        if (arg_parser.option_exists("--seed")) {
            seed = std::stoul(arg_parser.option_value("--seed"));
        } else {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }

        std::mt19937 rng(seed);
        rng.discard(10000);
        std::uniform_int_distribution<unsigned> x_dist(0, max_x);
        std::uniform_int_distribution<unsigned> y_dist(0, max_y);

        std::cout << "NAME: kro" << id << n << std::endl;
        std::cout << "TYPE: TSP" << std::endl;
        std::cout << "COMMENT: " << n << "-city problem " << id << " (Mendes)" << std::endl;
        std::cout << "DIMENSION: " << n << std::endl;
        std::cout << "EDGE_WEIGHT_TYPE : EUC_2D" << std::endl;
        std::cout << "NODE_COORD_SECTION" << std::endl;

        for (i = 1; i <= n; i++) {
            x = x_dist(rng);
            y = y_dist(rng);
            std::cout << i << " " << x << " " << y << std::endl;
        }
    }

    return 0;
}
