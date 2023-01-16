#include "solution/solution.hpp"
#include <algorithm>
#include <numeric>
#include <limits>

namespace motsp {
bool Solution::dominates(const std::vector<double> & costA,
                         const std::vector<double> & costB) {
    if(costA.size() != costB.size()) {
        return false;
    }

    // Checks if costA is at least as good as costB
    for(unsigned i = 0; i < costA.size(); i++) {
        if(costA[i] > costB[i] + std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }

    // Checks if costA is better than costB
    for(unsigned i = 0; i < costA.size(); i++) {
        if(costA[i] < costB[i] - std::numeric_limits<double>::epsilon()) {
            return true;
        }
    }

    return false;
}

void Solution::normalize_cycle() {
    std::vector<unsigned>::iterator it = find(this->cycle.begin(),
                                              this->cycle.end(),
                                              0);
    if(it != this->cycle.end()) {
        std::rotate(this->cycle.begin(), it, this->cycle.end());
    }

    if(this->cycle[1] > this->cycle.back()) {
        std::reverse(this->cycle.begin() + 1, this->cycle.end());
    }
}

void Solution::compute_cycle(const std::vector<double> & key) {
    std::vector<std::pair<double, unsigned>> permutation(key.size());

    for(unsigned i = 0; i < key.size(); i++) {
        permutation[i] = std::make_pair(key[i], i + 1);
    }

    std::sort(permutation.begin(), permutation.end());

    this->cycle = std::vector<unsigned>(key.size() + 1, key.size() + 1);

    this->cycle[0] = 0;

    for(unsigned i = 0; i < key.size(); i++) {
        this->cycle[i + 1] = permutation[i].second;
    }

    this->normalize_cycle();
}

void Solution::init() {
    // compute the cost
    this->cost = std::vector<double>(this->instance.num_objectives, 0.0);

    for(unsigned i = 0; i < this->instance.num_objectives; i++) {
        for(unsigned j = 0; j < this->instance.num_vertices - 1; j++) {
            this->cost[i] += this->instance.adj[i][this->cycle[j]][this->cycle[j + 1]];
        }

        this->cost[i] += this->instance.adj[i][this->cycle.back()][this->cycle.front()];
    }
}

Solution::Solution(const Instance & instance,
                   const std::vector<unsigned> & cycle) : instance(instance),
                                                          cycle(cycle) {
    this->normalize_cycle();
    this->init();
}

Solution::Solution(const Instance & instance,
                   const std::vector<double> & key) : instance(instance) {
    this->compute_cycle(key);
    this->init();
}

bool Solution::is_feasible() const {
    if(!this->instance.is_valid()) {
        return false;
    }

    for(unsigned i = 0; i < this->instance.num_objectives; i++) {
        if(this->cost[i] < 0.0) {
            return false;
        }

        if(this->cost[i] > this->instance.primal_bound[i]) {
            return false;
        }
    }

    std::vector<unsigned> sequence(this->instance.num_vertices, 0);
    std::iota(sequence.begin(), sequence.end(), 0);
    return std::is_permutation(this->cycle.begin(),
                               this->cycle.end(),
                               sequence.begin());
}

bool Solution::dominates(const Solution & solution) const {
    return Solution::dominates(this->cost, solution.cost);
}

std::istream & operator >>(std::istream & is, Solution & solution) {
    for(unsigned i = 0; i < solution.instance.num_vertices; i++) {
        is >> solution.cycle[i];
    }

    return is;
}

std::ostream & operator <<(std::ostream & os, Solution & solution) {
    for(unsigned i = 0; i < solution.instance.num_vertices - 1; i++) {
        os << solution.cycle[i] << ' ';
    }

    os << solution.cycle.back() << std::endl;

    return os;
}

}
