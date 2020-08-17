#pragma once

#include "../../instance/Instance.hpp"
#include <pagmo/types.hpp>

namespace MOTSP {
    class Problem {
        private:
            Instance instance;

        public:
            Problem(const Instance instance);

            Problem();

            pagmo::vector_double fitness(const pagmo::vector_double & dv) const;

            std::pair<pagmo::vector_double, pagmo::vector_double> get_bounds() const;

            pagmo::vector_double::size_type get_nobj() const;
    };
}

