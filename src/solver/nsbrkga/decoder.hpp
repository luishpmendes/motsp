#pragma once

#include "solution/solution.hpp"
#include "chromosome.hpp"

namespace motsp {

class Decoder {
    public:
    const Instance & instance;

    std::vector<std::vector<std::pair<double, unsigned>>> permutations;

    unsigned max_local_search_iterations;

    std::vector<std::vector<unsigned>> cycles;

    std::vector<std::vector<double>> costs;

    Decoder(const Instance & instance,
            unsigned num_threads,
            unsigned max_local_search_iterations = 0);

    std::vector<double> decode(NSBRKGA::Chromosome & chromosome, bool rewrite);
};

}
