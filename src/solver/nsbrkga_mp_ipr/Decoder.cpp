#include "Decoder.hpp"
#include "../local_search/TwoOpt.hpp"
#include <algorithm>

namespace MOTSP {
    Decoder::Decoder(const Instance & instance,
                     unsigned numThreads,
                     unsigned maxLocalSearchIterations)
        : instance(instance),
          permutations(numThreads,
                       std::vector<std::pair<double, unsigned>>(
                           instance.getNumVertices() - 1)),
          maxLocalSearchIterations(maxLocalSearchIterations),
          cycles(numThreads,
                 std::vector<unsigned>(instance.getNumVertices(), 0)),
          costs(numThreads,
                std::vector<double>(instance.getNumObjectives(), 0.0)) {}

    std::vector<double> Decoder::decode(BRKGA::Chromosome & chromosome, 
                                        bool rewrite) {
#       ifdef _OPENMP
            std::vector<std::pair<double, unsigned>> & permutation = 
                this->permutations[omp_get_thread_num()];
            std::vector<unsigned> & cycle = this->cycles[omp_get_thread_num()];
            std::vector<double> & cost = this->costs[omp_get_thread_num()];
#       else
            std::vector<std::pair<double, unsigned>> & permutation = 
                this->permutations.front();
            std::vector<unsigned> & cycle = this->cycles.front();
            std::vector<double> & cost = this->costs.front();
#       endif

        const unsigned & numVertices   = instance.getNumVertices(),
                       & numObjectives = instance.getNumObjectives();
        const std::vector<std::vector<std::vector<double>>> & adj =
            instance.getAdj();

        for(unsigned i = 0; i < numVertices - 1; i++) {
            permutation[i] = std::make_pair(chromosome[i], i + 1);
        }

        std::sort(permutation.begin(), permutation.end());

        cycle[0] = 0;

        for(unsigned i = 0; i < numVertices - 1; i++) {
            cycle[i + 1] = permutation[i].second;
        }

        if(rewrite && this->maxLocalSearchIterations > 0) {
            cycle = TwoOpt::improve(instance.getAdj(),
                                    cycle,
                                    this->maxLocalSearchIterations,
                                    1).front();

            std::sort(chromosome.begin(), chromosome.end());

            for(unsigned i = 0; i < numVertices - 1; i++) {
                permutation[i] = std::make_pair(cycle[i + 1], chromosome[i]);
            }

            std::sort(permutation.begin(), permutation.end());

            for(unsigned i = 0; i < numVertices - 1; i++) {
                chromosome[i] = permutation[i].second;
            }
        }

        for(unsigned j = 0; j < numObjectives; j++) {
            cost[j] = 0;
            for(unsigned i = 0; i < numVertices; i++) {
                cost[j] += adj[j][cycle[i]][cycle[(i + 1) % numVertices]];
            }
        }

        return cost;
    }
}

