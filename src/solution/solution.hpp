#pragma once

#include "instance/instance.hpp"

namespace motsp {
/***************************************************
 * The Solution class represents a solution for the
 * Multi-Objective Travelling Salesman Problem.
 ***************************************************/
class Solution {
    public:
    /**********************************************************
     * Returns true if costA dominates costB; false otherwise.
     *
     * @param costA the first cost been compared.
     * @param costB the second cost been compared.
     *
     * @return true if costA dominates costB; false otherwise.
     **********************************************************/
    static bool dominates(const std::vector<double> & costA,
                          const std::vector<double> & costB);

    /****************************
     * The instance been solved.
     ****************************/
    const Instance & instance;

    /****************************
     * The Hamiltonian cycle.
     ****************************/
    std::vector<unsigned> cycle;

    /*************************
     * The solution's cost.
     *************************/
    std::vector<double> cost;

    private:
    /************************
     * Normalizes the cycle.
     ************************/
    void normalize_cycle();

    /***************************************************
     * Computes the cycle from the key.
     *
     * @param key the key.
     ***************************************************/
    void compute_cycle(const std::vector<double> & key);

    /******************************
     * Initializes a new solution.
     ******************************/
    void init();

    public:
    /**********************************************
     * Constructs a new solution.
     *
     * @param instance the instance been solved.
     * @param cycle    the Hamiltonian cycle.
     **********************************************/
    Solution(const Instance & instance,
             const std::vector<unsigned> & cycle);

    /**************************************************************
     * Constructs a new solution.
     *
     * @param instance the instance been solved.
     * @param key      the key representing the Hamiltonian cycle.
     **************************************************************/
    Solution(const Instance & instance,
             const std::vector<double> & key);

    /********************************
     * Constructs an empty solution.
     ********************************/
    Solution();

    /*********************************************
     * Verifies whether this solution is
     * feasible for the instance been solved.
     *
     * @return true if this solution is feasible;
     *         false otherwise.
     *********************************************/
    bool is_feasible() const;

    /*******************************************************************
     * Verifies whether this solution dominates the specified one.
     *
     * @param solution the solution whose domination is to be verified.
     *
     * @return true if this instance dominated the specified one;
     *         false otherwise.
     *******************************************************************/
    bool dominates(const Solution & solution) const;

    /*******************************************************
     * Standard input operator.
     *
     * @param is       standard input stream object.
     * @param solution the solution.
     *
     * @return the stream object.
     *******************************************************/
    friend std::istream & operator >>(std::istream & is,
                                      Solution & solution);

    /*************************************************************
     * Standard output operator.
     *
     * @param os       standard output stream object.
     * @param solution the solution.
     *
     * @return the stream object.
     *************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Solution & solution);
};

}
