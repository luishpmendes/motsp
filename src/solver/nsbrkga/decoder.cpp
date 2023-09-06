#include "solver/nsbrkga/decoder.hpp"
#include "solver/local_search/two_opt.hpp"
#include <algorithm>

namespace motsp {

Decoder::Decoder(const Instance & instance,
                 unsigned num_threads,
                 unsigned max_local_search_iterations)
    : instance(instance),
      permutations(num_threads,
                   std::vector<std::pair<double, unsigned>>(
                       instance.num_vertices - 1)),
      max_local_search_iterations(max_local_search_iterations),
      cycles(num_threads,
             std::vector<unsigned>(instance.num_vertices, 0)),
      costs(num_threads,
            std::vector<double>(instance.num_objectives, 0.0)) {}

std::vector<double> Decoder::decode(NSBRKGA::Chromosome & chromosome,
                                    bool rewrite) {
#   ifdef _OPENMP
        std::vector<std::pair<double, unsigned>> & permutation =
            this->permutations[omp_get_thread_num()];
        std::vector<unsigned> & cycle = this->cycles[omp_get_thread_num()];
        std::vector<double> & cost = this->costs[omp_get_thread_num()];
#   else
        std::vector<std::pair<double, unsigned>> & permutation =
            this->permutations.front();
        std::vector<unsigned> & cycle = this->cycles.front();
        std::vector<double> & cost = this->costs.front();
#   endif

    for(unsigned i = 0; i < this->instance.num_vertices - 1; i++) {
        permutation[i] = std::make_pair(chromosome[i], i + 1);
    }

    std::sort(permutation.begin(), permutation.end());

    cycle[0] = 0;

    for(unsigned i = 0; i < this->instance.num_vertices - 1; i++) {
        cycle[i + 1] = permutation[i].second;
    }

    if(rewrite && this->max_local_search_iterations > 0) {
        cycle = Two_Opt::improve(this->instance.adj,
                                 cycle,
                                 this->max_local_search_iterations,
                                 1).front();

        std::sort(chromosome.begin(), chromosome.end());

        for(unsigned i = 0; i < this->instance.num_vertices - 1; i++) {
            permutation[i] = std::make_pair(cycle[i + 1], chromosome[i]);
        }

        std::sort(permutation.begin(), permutation.end());

        for(unsigned i = 0; i < this->instance.num_vertices - 1; i++) {
            chromosome[i] = permutation[i].second;
        }
    }

    for(unsigned i = 0; i < this->instance.num_objectives; i++) {
        cost[i] = 0;
        for(unsigned j = 0; j < this->instance.num_vertices - 1; j++) {
            cost[i] += this->instance.adj[i][cycle[j]][cycle[j + 1]];
        }
        cost[i] += this->instance.adj[i][cycle.back()][cycle.front()];
    }

    return cost;
}

}
