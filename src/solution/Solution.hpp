#pragma once

#include "../instance/Instance.hpp"

namespace MOTSP {
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
        private:
            /****************************
             * The Hamiltonian cycle.
             ****************************/
            std::vector<unsigned> cycle;

            /*************************
             * The solution's cost.
             *************************/
            std::vector<double> cost;

            /************************
             * Normalizes the cycle.
             ************************/
            void normalizeCycle();

            /***************************************************
             * Computes the cycle from the key.
             *
             * @param key the key.
             ***************************************************/
            void computeCycle(const std::vector<double> & key);

            /********************************************
             * Initializes a new solution.
             *
             * @param instance the instance been solved.
             ********************************************/
            void init(const Instance & instance);

            /********************************************************
             * Initializes a new solution
             *
             * @param instance the instance been solved.
             * @param is       the input stream to read from.
             ********************************************************/
            void init(const Instance & instance, std::istream & is);

        public:
            /*********************************************
             * Constructs a new solution.
             *
             * @param cycle the Hamiltonian cycle.
             * @param cost  the solution cost.
             *********************************************/
            Solution(const std::vector<unsigned> & cycle,
                     const std::vector<double> & cost);

            /*******************************************
             * Constructs a new solution.
             *
             * @param key  the key.
             * @param cost the solution cost.
             *******************************************/
            Solution(const std::vector<double> & key,
                     const std::vector<double> & cost);

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

            /*******************************************************
             * Constructs a new solution.
             *
             * @param instance the instance been solved.
             * @param is       the input stream to read from.
             *******************************************************/
            Solution(const Instance & instance, std::istream & is);

            /***********************************************************
             * Constructs a new solution.
             *
             * @param instance the instance been solved.
             * @param filename the file to read from.
             ***********************************************************/
            Solution(const Instance & instance, const char * filename);

            /******************************************************************
             * Constructs a new solution.
             *
             * @param instance the instance been solved.
             * @param filename the file to read from.
             ******************************************************************/
            Solution(const Instance & instance, const std::string & filename);

            /***********************************
             * Constructs a new empty solution.
             ***********************************/
            Solution();

            /***********************************************
             * Returns the Hamiltonian cycle.
             *
             * @return the Hamiltonian cycle.
             ***********************************************/
            const std::vector<unsigned> & getCycle() const;

            /********************************************
             * Returns the solution's cost.
             *
             * @return the solution's cost.
             ********************************************/
            const std::vector<double> & getCost() const;

            /***************************************************************
             * Verifies whether this solution is feasible for the specified
             * instance.
             *
             * @param instance the instance been solved.
             *
             * @return true if this instance is feasible; false otherwise.
             ***************************************************************/
            bool isFeasible(const Instance & instance) const;

            /**************************************************************
             * Verifies whether this solution dominates the specified one.
             *
             * @param solution the solution whose domination is to be 
             *        verified.
             *
             * @return true if this instance dominated the specified one; 
             *         false otherwise.
             **************************************************************/
            bool dominates(const Solution & solution) const;

            /********************************************************
             * Write this solution into the specified output stream.
             *
             * @param os the output stream to write into.
             ********************************************************/
            void write(std::ostream & os) const;

            /***********************************************
             * Write this solution into the specified file.
             *
             * @param filename the file to write into.
             ***********************************************/
            void write(const char * filename) const;

            /***********************************************
             * Write this solution into the specified file.
             *
             * @param filename the file to write into.
             ***********************************************/
            void write(const std::string & filename) const;

            /*******************************************************
             * Write this solution into the standard output stream.
             *******************************************************/
            void write() const;
    };
}

