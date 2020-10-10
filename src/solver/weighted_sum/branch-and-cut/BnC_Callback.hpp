#pragma once

#include "gurobi_c++.h"

namespace MOTSP {
    class BnC_Callback : public GRBCallback {
        public:
            static void findsubtour(unsigned numVertices,
                                    std::vector<std::vector<double>> xVal,
                                    unsigned * len,
                                    std::vector<unsigned> & tour);

        private:
            std::vector<std::vector<GRBVar>> x;
            unsigned numVertices;

        protected:
            void callback();

        public:
            BnC_Callback(std::vector<std::vector<GRBVar>> x,
                         unsigned numVertices);
    };
}

