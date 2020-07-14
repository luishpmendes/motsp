#include "Instance.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

namespace MOTSP {
    void Instance::init() {
        this->adj =
            std::vector<std::vector<std::vector<double>>>(this->numObjectives,
                    std::vector<std::vector<double>>(this->numVertices,
                        std::vector<double>(this->numVertices, 0.0)));

        for(unsigned i = 0; i < this->numObjectives; i++) {
            for(unsigned u = 0; u < this->numVertices; u++) {
                for(unsigned v = 0; v < this->numVertices; v++) {
                    this->adj[i][u][v] += 
                        (this->coord[i][u].first - this->coord[i][v].first) * 
                        (this->coord[i][u].first - this->coord[i][v].first);

                    this->adj[i][u][v] += 
                        (this->coord[i][u].second - this->coord[i][v].second) *
                        (this->coord[i][u].second - this->coord[i][v].second);

                    this->adj[i][u][v] = round(sqrt(this->adj[i][u][v]));
                }
            }
        }
    }

    void Instance::init(std::istream & is) {
        is >> this->numObjectives >> this->numVertices;

        this->coord = std::vector<std::vector<std::pair<double, double>>>(
                this->numObjectives, std::vector<std::pair<double, double>>(
                    this->numVertices));

        for(unsigned v = 0; v < this->numVertices; v++) {
            for(unsigned i = 0; i < this->numObjectives; i++) {
                is >> this->coord[i][v].first >> this->coord[i][v].second;
            }
        }

        this->init();
    }

    Instance::Instance(
            const std::vector<std::vector<std::vector<double>>> & adj) 
        : adj(adj) {
        this->numObjectives = this->adj.size();
        this->numVertices = this->adj.front().size();
    }

    Instance::Instance(
            const std::vector<std::vector<std::pair<double, double>>> & coord) 
        : coord(coord) {
        this->numObjectives = this->coord.size();
        this->numVertices = this->coord.front().size();

        this->init();
    }

    Instance::Instance(std::istream & is) {
        this->init(is);
    }

    Instance::Instance(const char * filename) {
        std::ifstream ifs;
        ifs.open(filename);

        if(ifs.is_open()) {
            this->init(ifs);
        } else {
            throw "File not found.";
        }
    }

    Instance::Instance(const std::string & filename) 
        : Instance::Instance(filename.c_str()) {}

    Instance::Instance() {}

    unsigned Instance::getNumObjectives() const {
        return this->numObjectives;
    }

    unsigned Instance::getNumVertices() const {
        return this->numVertices;
    }

    const std::vector<std::vector<std::pair<double, double>>> & Instance::getCoord() const {
        return this->coord;
    }

    const std::vector<std::vector<std::vector<double>>> & Instance::getAdj() const {
        return this->adj;
    }

    bool Instance::isValid() const {
        if(this->adj.size() != this->numObjectives) {
            return false;
        }

        for(unsigned i = 0; i < this->numObjectives; i++) {
            if(this->adj[i].size() != this->numVertices) {
                return false;
            }
        }

        for(unsigned i = 0; i < this->numObjectives; i++) {
            for(unsigned v = 0; v < this->numVertices; v++) {
                if(this->adj[i][v].size() != this->numVertices) {
                    return false;
                }
            }
        }

        return true;
    }

    void Instance::write(std::ostream & os) const {
        os << this->numObjectives << " " 
           << this->numVertices << std::endl;;

        for(unsigned v = 0; v < this->numVertices; v++) {
            for(unsigned i = 0; i < this->numObjectives - 1; i++) {
                os << this->coord[i][v].first << " " 
                   << this->coord[i][v].second << " ";
            }

            os << this->coord[this->numObjectives - 1][v].first << " " 
               << this->coord[this->numObjectives - 1][v].second << std::endl;
        }
    }

    void Instance::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if(ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void Instance::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void Instance::write() const {
        this->write(std::cout);
    }
}

