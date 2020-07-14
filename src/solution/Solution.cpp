#include "Solution.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

namespace MOTSP {
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

    void Solution::normalizeCycle() {
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

    void Solution::computeCycle(const std::vector<double> & key) {
        std::vector<std::pair<double, unsigned>> permutation(key.size());

        for(unsigned i = 0; i < key.size(); i++) {
            permutation[i] = std::make_pair(key[i], i + 1);
        }

        std::sort(permutation.begin(), permutation.end());

        this->cycle = std::vector<unsigned>(key.size() + 1,
                                            key.size() + 1);

        this->cycle[0] = 0;

        for(unsigned i = 0; i < key.size(); i++) {
            this->cycle[i + 1] = permutation[i].second;
        }

        this->normalizeCycle();
    }

    void Solution::init(const Instance & instance) {
        // compute the cost
        this->cost = std::vector<double>(instance.getNumObjectives(), 0.0);
        for(unsigned i = 0; i < instance.getNumObjectives(); i++) {
            for(unsigned j = 0; j < instance.getNumVertices(); j++) {
                unsigned k = (j + 1) % instance.getNumVertices();
                unsigned u = this->cycle[j];
                unsigned v = this->cycle[k];

                this->cost[i] += instance.getAdj()[i][u][v];
            }
        }
    }

    void Solution::init(const Instance & instance, std::istream & is) {
        this->cycle = std::vector<unsigned>(instance.getNumVertices(),
                                            instance.getNumVertices());

        for(unsigned i = 0; i < instance.getNumVertices(); i++) {
            is >> this->cycle[i];
        }

        this->normalizeCycle();
        this->init(instance);
    }

    Solution::Solution(const std::vector<unsigned> & cycle,
                        const std::vector<double> & cost) : cycle(cycle),
                                                            cost(cost) {
        this->normalizeCycle();
    }

    Solution::Solution(const std::vector<double> & key,
                       const std::vector<double> & cost) : cost(cost) {
        this->computeCycle(key);
    }

    Solution::Solution(const Instance & instance, 
                       const std::vector<unsigned> & cycle) : cycle(cycle) {
        this->normalizeCycle();
        this->init(instance);
    }

    Solution::Solution(const Instance & instance, 
                       const std::vector<double> & key) {
        this->computeCycle(key);
        this->init(instance);
    }

    Solution::Solution(const Instance & instance, std::istream & is) {
        this->init(instance, is);
    }

    Solution::Solution(const Instance & instance, const char * filename) {
        std::ifstream ifs;

        ifs.open(filename);

        if(ifs.is_open()) {
            this->init(instance, ifs);
        } else {
            throw "File not found.";
        }
    }

    Solution::Solution(const Instance & instance, const std::string & filename)
        : Solution::Solution(instance, filename.c_str()) {}

    Solution::Solution() {}

    const std::vector<unsigned> & Solution::getCycle() const {
        return this->cycle;
    }

    const std::vector<double> & Solution::getCost() const {
        return this->cost;
    }

    bool Solution::isFeasible(const Instance & instance) const {
        std::vector<bool> isVertexUsed(instance.getNumVertices(), false);

        for(unsigned v : this->cycle) {
            if(isVertexUsed[v]) {
                return false;
            } else {
                isVertexUsed[v] = true;
            }
        }

        for(unsigned v = 0; v < instance.getNumVertices(); v++) {
            if(!isVertexUsed[v]) {
                return false;
            }
        }

        return true;
    }

    bool Solution::dominates(const Solution & solution) const {
        return Solution::dominates(this->cost, solution.cost);
    }

    void Solution::write(std::ostream & os) const {
        for(unsigned i = 0; i < this->cycle.size() - 1; i++) {
            os << this->cycle[i] << " ";
        }

        os << this->cycle.back() << std::endl;
    }

    void Solution::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if(ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void Solution::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void Solution::write() const {
        this->write(std::cout);
    }
}

