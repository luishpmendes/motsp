#include "Problem.hpp"
#include <algorithm>

namespace MOTSP {
    Problem::Problem(const Instance instance) : instance(instance) {}

    Problem::Problem() {}

    pagmo::vector_double Problem::fitness(const pagmo::vector_double & dv) const {
        const unsigned & numVertices   = instance.getNumVertices(),
                       & numObjectives = instance.getNumObjectives();
        const std::vector<std::vector<std::vector<double>>> & adj =
            instance.getAdj();

        std::vector<std::pair<double, unsigned>> permutation(numVertices - 1);
        std::vector<unsigned> cycle(numVertices, 0);
        std::vector<double> cost(numObjectives, 0.0);


        for(unsigned i = 0; i < numVertices - 1; i++) {
            permutation[i] = std::make_pair(dv[i], i + 1);
        }

        std::sort(permutation.begin(), permutation.end());

        cycle[0] = 0;

        for(unsigned i = 0; i < numVertices - 1; i++) {
            cycle[i + 1] = permutation[i].second;
        }

        for(unsigned i = 0; i < numVertices; i++) {
            for(unsigned j = 0; j < numObjectives; j++) {
                cost[j] += adj[j][cycle[i]][cycle[(i + 1) % numVertices]];
            }
        }

        return cost;
    }

    std::pair<pagmo::vector_double, pagmo::vector_double> Problem::get_bounds() const {
        return std::make_pair(
                pagmo::vector_double(this->instance.getNumVertices() - 1, 0.0),
                pagmo::vector_double(this->instance.getNumVertices() - 1, 1.0));
    }

    pagmo::vector_double::size_type Problem::get_nobj() const {
        return this->instance.getNumObjectives();
    }
}

