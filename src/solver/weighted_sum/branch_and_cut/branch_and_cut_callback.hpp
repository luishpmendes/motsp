#pragma once

#include "gurobi_c++.h"

namespace motsp {
class Branch_and_Cut_Callback : public GRBCallback {
    public:
        static void find_subtour(unsigned num_vertices,
                                 std::vector<std::vector<double>> x_val,
                                 unsigned * tour_len,
                                 std::vector<unsigned> & tour);

        std::vector<std::vector<GRBVar>> x;
        unsigned num_vertices;

    protected:
        void callback();

    public:
        Branch_and_Cut_Callback(std::vector<std::vector<GRBVar>> x,
                                unsigned numVertices);
};

}

