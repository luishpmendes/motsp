#include "BnC_Callback.hpp"

namespace MOTSP {
    void BnC_Callback::findsubtour(unsigned numVertices,
                                   std::vector<std::vector<double>> xVal,
                                   unsigned * tourLen,
                                   std::vector<unsigned> & tour) {
        std::vector<bool> seen(numVertices, false);
        int bestind = -1;
        unsigned bestlen = numVertices + 1,
                 start = 0,
                 node = 0;

        while(start < numVertices) {
            for(node = 0; node < numVertices; node++) {
                if(!seen[node]) {
                    break;
                }
            }

            if(node == numVertices) {
                break;
            }

            for(unsigned len = 0; len < numVertices; len++) {
                unsigned i = 0;
                tour[start + len] = node;
                seen[node] = true;
                for(i = 0; i < numVertices; i++) {
                    if(xVal[node][i] > 0.5 && !seen[i]) {
                        node = i;
                        break;
                    }
                }

                if(i == numVertices) {
                    len++;

                    if(len < bestlen) {
                        bestlen = len;
                        bestind = start;
                    }

                    start += len;
                    break;
                }
            }
        }

        for(unsigned i = 0; i < bestlen; i++) {
            tour[i] = tour[bestind + i];
        }

        *tourLen = bestlen;
    }

    BnC_Callback::BnC_Callback(std::vector<std::vector<GRBVar>> x,
                               unsigned numVertices)
        : x(x), numVertices(numVertices) {}

    void BnC_Callback::callback() {
        try {
            if (where == GRB_CB_MIPSOL) {
                std::vector<std::vector<double>> xVal(this->numVertices,
                        std::vector<double>(this->numVertices, 0.0));
                std::vector<unsigned> tour(this->numVertices);
                unsigned len;

                for(unsigned u = 0; u < this->numVertices; u++) {
                    for(unsigned v = 0; v < this->numVertices; v++) {
                        xVal[u][v] = this->getSolution(this->x[u][v]);
                    }
                }

                BnC_Callback::findsubtour(this->numVertices,
                                          xVal,
                                          &len,
                                          tour);

                if(len < this->numVertices) {
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

