#pragma once

#include "solver/solver.hpp"

namespace motsp {
/********************************************************
 * The Christofides_Solver represents a solver for the
 * Multi-Objective Travelling Salesman Problem using the
 * Weighted Sum Method with the Christofies Algorithm.
 ********************************************************/
class Christofides_Solver : public Solver {
    public:
    /****************************************************************
     * All individuals found during optimisation.
     ****************************************************************/
    std::vector<std::pair<std::vector<double>, std::vector<double>>>
        all_individuals;

    /*********************************************
     * Constructs a new solver.
     *
     * @param instance the instance to be solved.
     *********************************************/
    Christofides_Solver(const Instance & instance);

    /******************************
     * Constructs an empty solver.
     ******************************/
    Christofides_Solver();

    /***************************************************************
     * Capture a snapshot of the current state of the optimization.
     ***************************************************************/
    void capture_snapshot();

    /**********************
     * Solve the instance.
     **********************/
    void solve();

    /**********************************************************************
     * Standard stream operator.
     *
     * @param os the standard output stream object.
     * @param solver the solver.
     *
     * @return the stream object.
     **********************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Christofides_Solver & solver);
};

}

