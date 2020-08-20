#pragma once

#include "../Solver.hpp"
#include <pagmo/population.hpp>

namespace MOTSP {
    /**************************************************************************
     * The NSGA2_Solver represents a solver for the Multi-Objective Travelling
     * Salesman Problem using the Non-Dominated Sorting Genetic Algorithm II.
     **************************************************************************/
    class NSGA2_Solver : public Solver {
        private:
            /**************************
             * Size of the population.
             **************************/
            unsigned populationSize;

            /*************************************************
             * The method used to generate initial individuals.
             *************************************************/
            unsigned initialIndividualsMethod;

            /*******************************************************************
             * The percentage of the initial population that will be generated.
             *******************************************************************/
            double initialIndividualsPercentage;

            /****************************************************************
             * The percentage of the time limit used to generate the initial
             * individuals.
             ****************************************************************/
            double initialIndividualsTimePercentage;

            /*************************
             * Crossover probability.
             *************************/
            double crossoverProbability;

            /************************************
             * Distribution index for crossover.
             ************************************/
            double crossoverDistribution;

            /***************************
             * Mutation probability.
             ***************************/
            double mutationProbability;

            /***********************************
             * Distribution index for mutation.
             ***********************************/
            double mutationDistribution;

            /*****************************
             * The number of generations.
             *****************************/
            unsigned numGenerations;

            /******************************************************
             * Update the best individuals found so far.
             *
             * @param pop the new solutions.
             *
             * @return true if the best individuals are modified;
             *         false otherwise.
             ******************************************************/
            bool updateIndividuals(const pagmo::population & pop);

        public:
            /*******************************************************************
             * Constructs a new solver.
             *
             * @param instance                         the instance to be
             *                                         solved.
             * @param seed                             the seed for the
             *                                         pseudo-random numbers
             *                                         generator.
             * @param timeLimit                        the time limit in
             *                                         seconds.
             * @param maxNumSolutions                  the maximum number of
             *                                         solutions.
             * @param captureEnabled                   the flag indicating
             *                                         whether the capturing of
             *                                         solutions for evaluation
             *                                         is enabled.
             * @param populationSize                   the size of the
             *                                         population.
             * @param initialIndividualsMethod         the method used to
             *                                         generate initial
             *                                         solutions.
             * @param initialIndividualsPercentage     the percentage of the
             *                                         initial population that
             *                                         will be generated.
             * @param initialIndividualsTimePercentage the percentage of the
             *                                         time limit used to
             *                                         generate the initial
             *                                         solutions.
             * @param crossoverProbability             the crossover
             *                                         probability.
             * @param crossoverDistribution            the distribution index
             *                                         for crossover.
             * @param mutationProbability              the mutation
             *                                         probability.
             * @param mutationDistribution             the distribution index
             *                                         for mutation.
             *******************************************************************/
            NSGA2_Solver(const Instance & instance,
                         unsigned seed,
                         double timeLimit,
                         unsigned maxNumSolutions,
                         bool captureEnabled,
                         unsigned populationSize,
                         unsigned initialIndividualsMethod = 0,
                         double initialIndividualsPercentage = 0.0,
                         double initialIndividualsTimePercentage = 0.0,
                         double crossoverProbability = 0.95,
                         double crossoverDistribution = 10.00,
                         double mutationProbability = 0.01,
                         double mutationDistribution = 50.00);

            /*******************************************************************
             * Constructs a new solver.
             *
             * @param instance        the instance to be solved.
             * @param timeLimit       the time limit in seconds.
             * @param maxNumSolutions the maximum number of solutions.
             * @param captureEnabled  the flag indicating whether the capturing
             *                        of solutions for evaluation is enabled.
             *******************************************************************/
            NSGA2_Solver(const Instance & instance,
                         unsigned seed,
                         double timeLimit = 3600.0,
                         unsigned maxNumSolutions =
                                std::numeric_limits<unsigned>::max(),
                         bool captureEnabled = false);

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

            /**************************************
             * Returns the size of the population.
             *
             * @return the size of the population.
             **************************************/
            unsigned getPopulationSize() const;

            /********************************************************
             * Sets the size of the population.
             *
             * @param populationSize the new size of the population.
             ********************************************************/
            void setPopulationSize(unsigned populationSize);

            /***********************************************************
             * Returns the method used to generate initial individuals.
             *
             * @return the method used to generate initial individuals.
             ***********************************************************/
            unsigned getInitialIndividualsMethod() const;

            /********************************************************************
             * Sets the method used to generate initial individuals.
             *
             * @param initialIndividualsMethod the new method used to generate
             *        initial individuals.
             ********************************************************************/
            void setInitialIndividualsMethod(unsigned initialIndividualsMethod);

            /****************************************************************
             * Returns the percentage of the initial population that will be
             * generated.
             *
             * @return the percentage of the initial population that will be
             *         generated.
             ****************************************************************/
            double getInitialIndividualsPercentage() const;

            /****************************************************************
             * Sets the percentage of the initial population that will be
             * generated.
             *
             * @param initialIndividualsPercentage the new percentage of the
             *        initial population that will be generated. 
             ****************************************************************/
            void setInitialIndividualsPercentage(
                    double initialIndividualsPercentage);

            /****************************************************************
             * Returns the percentage of the time limit used to generate the
             * initial individuals.
             *
             * @return the percentage of the time limit used to generate the
             *         initial individuals.
             ****************************************************************/
            double getInitialIndividualsTimePercentage() const;

            /****************************************************************
             * Sets the percentage of the time limit used to generate the
             * initial solutions.
             *
             * @param initialIndividualsTimePercentage the new percentage of
             *        the time limit used to generate the initial solutions.
             ****************************************************************/
            void setInitialIndividualsTimePercentage(
                    double initialIndividualsTimePercentage);

            /***************************************
             * Returns the crossover probability.
             *
             * @return the crossover probability.
             ***************************************/
            double getCrossoverProbability() const;

            /*************************************************************
             * Sets the crossover probability.
             *
             * @param crossoverProbability the new crossover probability.
             *************************************************************/
            void setCrossoverProbability(double crossoverProbability);

            /************************************************
             * Returns the distribution index for crossover.
             *
             * @return the distribution index for crossover.
             ************************************************/
            double getCrossoverDistribution() const;

            /**************************************************************
             * Sets the distribution index for crossover.
             *
             * @param crossoverDistribution the new distribution index for
             *        crossover.
             **************************************************************/
            void setCrossoverDistribution(double crossoverDistribution);

            /**************************************
             * Returns the mutation probability.
             *
             * @return the mutation probability.
             **************************************/
            double getMutationProbability() const;

            /************************************************************
             * Sets the mutation probability.
             *
             * @param mutationProbability the new crossover probability.
             ************************************************************/
            void setMutationProbability(double mutationProbability);

            /***********************************************
             * Returns the distribution index for mutation.
             *
             * @return the distribution index for mutation.
             ***********************************************/
            double getMutationDistribution() const;

            /*************************************************************
             * Sets the distribution index for mutation.
             *
             * @param mutationDistribution the new distribution index for
             *        mutation.
             *************************************************************/
            void setMutationDistribution(double mutationDistribution);

            /*************************************
             * Returns the number of generations.
             *
             * @return the number of generations.
             *************************************/
            unsigned getNumGenerations() const;

            /**********************
             * Solve the instance.
             **********************/
            void solve();

            /*********************************************************
             * Write the statistics into the specified output stream.
             *
             * @param os the output stream to write into.
             *********************************************************/
            void write(std::ostream & os) const;

            /************************************************
             * Write the statistics into the specified file.
             *
             * @param filename the file to write into.
             ************************************************/
            void write(const char * filename) const;

            /************************************************
             * Write the statistics into the specified file.
             *
             * @param filename the file to write into.
             ************************************************/
            void write(const std::string & filename) const;

            /********************************************************
             * Write the statistics into the standard output stream.
             ********************************************************/
            void write() const;
    };
}

