#pragma once

#include "solver/solver.hpp"

namespace motsp {
/****************************************************
 * The NSGA2_Solver represents a solver for the
 * Multi-Objective Travelling Salesman Problem using
 * the Non-Dominated Sorting Genetic Algorithm II.
 ****************************************************/
class NSGA2_Solver : public Solver {
    public:
    /********************************
     * Size of the population.
     ********************************/
    unsigned population_size = 300;

    /************************************
     * Crossover probability.
     ************************************/
    double crossover_probability = 0.95;

    /**************************************
     * Distribution index for crossover.
     **************************************/
    double crossover_distribution = 10.00;

    /***********************************
     * Mutation probability.
     ***********************************/
    double mutation_probability = 0.01;

    /*************************************
     * Distribution index for mutation.
     *************************************/
    double mutation_distribution = 50.00;

    /*********************************************
     * Constructs a new solver.
     *
     * @param instance the instance to be solved.
     *********************************************/
    NSGA2_Solver(const Instance & instance);

    /******************************
     * Constructs an empty solver.
     ******************************/
    NSGA2_Solver();

    /**********************
     * Solve the instance.
     **********************/
    void solve();

    /***************************************************************
     * Standard stream operator.
     *
     * @param os the standard output stream object.
     * @param solver the solver.
     *
     * @return the stream object.
     ***************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const NSGA2_Solver & solver);
};

}
