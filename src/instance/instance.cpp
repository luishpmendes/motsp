#include "instance/instance.hpp"
#include <algorithm>
#include <cmath>

namespace motsp {

void Instance::init() {
    this->adj =
        std::vector<std::vector<std::vector<double>>>(
                this->num_objectives, std::vector<std::vector<double>>(
                    this->num_vertices,
                    std::vector<double>(this->num_vertices, 0.0)));

    std::vector<std::vector<double>> edge_weight(this->num_objectives);

    this->primal_bound = std::vector<double>(this->num_objectives, 0.0);

    this->senses = std::vector<BRKGA::Sense>(this->num_objectives,
            BRKGA::Sense::MINIMIZE);

    for(unsigned i = 0; i < this->num_objectives; i++) {
        for(unsigned u = 0; u < this->num_vertices; u++) {
            for(unsigned v = 0; v < this->num_vertices; v++) {
                this->adj[i][u][v] +=
                    (this->coord[i][u].first - this->coord[i][v].first) *
                    (this->coord[i][u].first - this->coord[i][v].first);

                this->adj[i][u][v] +=
                    (this->coord[i][u].second - this->coord[i][v].second) *
                    (this->coord[i][u].second - this->coord[i][v].second);

                this->adj[i][u][v] = round(sqrt(this->adj[i][u][v]));

                edge_weight[i].push_back(this->adj[i][u][v]);
            }
        }

        std::sort(edge_weight[i].rbegin(), edge_weight[i].rend());

        for(unsigned u = 0; u < this->num_vertices; u++) {
            this->primal_bound[i] += edge_weight[i][u];
        }
    }
}

Instance::Instance(const std::vector<std::vector<std::vector<double>>> & adj) :
    adj(adj) {
    this->num_objectives = this->adj.size();
    this->num_vertices = this->adj.front().size();
    this->senses = std::vector<BRKGA::Sense>(this->num_objectives,
            BRKGA::Sense::MINIMIZE);
}

Instance::Instance(
        const std::vector<std::vector<std::pair<double, double>>> & coord) :
    coord(coord) {
    this->num_objectives = this->coord.size();
    this->num_vertices = this->coord.front().size();

    this->init();
}

Instance::Instance() {}

bool Instance::is_valid() const {
    if(this->adj.size() != this->num_objectives) {
        return false;
    }

    for(unsigned i = 0; i < this->num_objectives; i++) {
        if(this->adj[i].size() != this->num_vertices) {
            return false;
        }

        for(unsigned u = 0; u < this->num_vertices; u++) {
            if(this->adj[i][u].size() != this->num_vertices) {
                return false;
            }

            if(this->primal_bound[i] < 0.0) {
                return false;
            }

            for(unsigned v = 0; v < this->num_vertices; v++) {
                if(this->adj[i][u][v] < 0.0) {
                    return false;
                }
            }
        }
    }

    return true;
}

std::istream & operator >>(std::istream & is, Instance & instance) {
    is >> instance.num_objectives >> instance.num_vertices;

    instance.coord = std::vector<std::vector<std::pair<double, double>>>(
            instance.num_objectives,
            std::vector<std::pair<double, double>>(instance.num_vertices));

    for(unsigned v = 0; v < instance.num_vertices; v++) {
        for(unsigned i = 0; i < instance.num_objectives; i++) {
            is >> instance.coord[i][v].first >> instance.coord[i][v].second;
        }
    }

    instance.init();

    return is;
}

std::ostream & operator <<(std::ostream & os, Instance & instance) {
    os << instance.num_objectives << ' '
       << instance.num_vertices << std::endl;

    for(unsigned v = 0; v < instance.num_vertices; v++) {
        for(unsigned i = 0; i < instance.num_objectives - 1; i++) {
            os << instance.coord[i][v].first << ' '
               << instance.coord[i][v].second << ' ';
        }

        os << instance.coord[instance.num_objectives - 1][v].first << ' '
           << instance.coord[instance.num_objectives - 1][v].second
           << std::endl;
    }

    return os;
}

}
