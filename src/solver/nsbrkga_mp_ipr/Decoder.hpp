#pragma once

#include "../../solution/Solution.hpp"
#include "chromosome.hpp"

namespace MOTSP {
    class Decoder {
        private:
            Instance instance;

            std::vector<std::vector<std::pair<double, unsigned>>> permutations;

            unsigned maxLocalSearchIterations;

            std::vector<std::vector<unsigned>> cycles;

            std::vector<std::vector<double>> costs;

        public:
            Decoder(const Instance & instance,
                    unsigned numThreads,
                    unsigned maxLocalSearchIterations = 0);

            std::vector<double> decode(BRKGA::Chromosome & chromosome, 
                                       bool rewrite);
    };
}

