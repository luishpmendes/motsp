#include "solver/ihs/problem.hpp"
#include "solution/solution.hpp"

namespace motsp {

Problem::Problem(const Instance & instance) : instance(instance) {}

Problem::Problem() {}

pagmo::vector_double Problem::fitness(const pagmo::vector_double & dv) const {
    Solution solution(this->instance, dv);
    return solution.cost;
}

std::pair<pagmo::vector_double, pagmo::vector_double> Problem::get_bounds()
    const {
    return std::make_pair(
            pagmo::vector_double(this->instance.num_vertices - 1, 0.0),
            pagmo::vector_double(this->instance.num_vertices - 1, 1.0));
}

pagmo::vector_double::size_type Problem::get_nobj() const {
    return this->instance.num_objectives;
}

}
