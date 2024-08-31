#include "solver/weighted_sum/branch_and_cut/branch_and_cut_callback.hpp"

namespace motsp {
void Branch_and_Cut_Callback::find_subtour(
        unsigned num_vertices,
        std::vector<std::vector<double>> x_val,
        unsigned * tour_len,
        std::vector<unsigned> & tour) {
    std::vector<bool> seen(num_vertices, false);
    int best_ind = -1;
    unsigned best_len = num_vertices + 1,
             start = 0,
             node = 0;

    while(start < num_vertices) {
        for(node = 0; node < num_vertices; node++) {
            if(!seen[node]) {
                break;
            }
        }

        if(node == num_vertices) {
            break;
        }

        for(unsigned len = 0; len < num_vertices; len++) {
            unsigned i = 0;
            tour[start + len] = node;
            seen[node] = true;
            for(i = 0; i < num_vertices; i++) {
                if(x_val[node][i] > 0.5 && !seen[i]) {
                    node = i;
                    break;
                }
            }

            if(i == num_vertices) {
                len++;

                if(len < best_len) {
                    best_len = len;
                    best_ind = start;
                }

                start += len;
                break;
            }
        }
    }

    for(unsigned i = 0; i < best_len; i++) {
        tour[i] = tour[best_ind + i];
    }

    *tour_len = best_len;
}

Branch_and_Cut_Callback::Branch_and_Cut_Callback(
        std::vector<std::vector<GRBVar>> x,
        unsigned num_vertices)
    : x(x), num_vertices(num_vertices) {}

void Branch_and_Cut_Callback::callback() {
    try {
        if (this->where == GRB_CB_MIPSOL) {
            std::vector<std::vector<double>> x_val(
                    this->num_vertices,
                    std::vector<double>(this->num_vertices, 0.0));
            std::vector<unsigned> tour(this->num_vertices);
            unsigned len;

            for(unsigned u = 0; u < this->num_vertices; u++) {
                for(unsigned v = 0; v < this->num_vertices; v++) {
                    x_val[u][v] = this->getSolution(this->x[u][v]);
                }
            }

            Branch_and_Cut_Callback::find_subtour(this->num_vertices,
                                                  x_val,
                                                  &len,
                                                  tour);

            if(len < this->num_vertices) {
                GRBLinExpr lhs = 0.0;

                for(unsigned i = 0; i < len; i++) {
                    for(unsigned j = i + 1; j < len; j++) {
                        lhs += this->x[tour[i]][tour[j]];
                    }
                }

                this->addLazy(lhs <= len-1);
            }
        }
    } catch(GRBException e) {
        std::cout << "Error number: " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    } catch(...) {
        std::cout << "Error during callback" << std::endl;
    }
}

}

