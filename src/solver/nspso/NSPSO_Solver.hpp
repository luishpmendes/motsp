#pragma once

#include "../Solver.hpp"
#include <pagmo/population.hpp>

namespace MOTSP {
    /**************************************************************************
     * The NSPSO_Solver represents a solver for the Multi-Objective Travelling
     * Salesman Problem using the Non-Dominated Sorting Particle Swap Optmizer.
     **************************************************************************/
    class NSPSO_Solver : public Solver {
        private:
            /**************************
             * Size of the population.
             **************************/
            unsigned populationSize;

            /***************************************************
             * The method used to generate initial individuals.
             ***************************************************/
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

            /*****************************
             * Particles' inertia weight.
             *****************************/
            double omega;

            /**************************************************************
             * Magnitude of the force, applied to the particle’s velocity,
             * in the direction of its previous best position.
             **************************************************************/
            double c1;

            /**************************************************************
             * Magnitude of the force, applied to the particle’s velocity,
             * in the direction of its global best (i.e., leader).
             **************************************************************/
            double c2;

            /***************************
             * Velocity scaling factor.
             ***************************/
            double chi;

            /*******************************************************
             * Velocity coefficient 
             * (determining the maximum allowed particle velocity).
             *******************************************************/
            double vCoeff;

            /****************************************************************
             * Leader selection range parameter 
             * (i.e., the leader of each particle is selected among the best
             * leader_selection_range % individuals).
             ****************************************************************/
            unsigned leaderSelectionRange;

            /************************************************************
             * The diversity mechanism used to maintain diversity on the
             * Pareto front.
             ************************************************************/
            std::string diversityMechanism;

            /*************************************************************
             * Memory parameter. 
             * If true, memory is activated in the algorithm for multiple
             * calls.
             *************************************************************/
            bool memory;

            /*****************************
             * The number of generations.
             *****************************/
            unsigned numGenerations;

            /******************************************************
             * Update the best individuals found so far.
             *
             * @param pop the new individuals.
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
             *                                         individuals.
             * @param omega                            the particles' inertia
             *                                         weight.
             * @param c1                               the magnitude of the
             *                                         force, applied to the
             *                                         particle's velocity, in
             *                                         the direction of its
             *                                         previous best position.
             * @param c2                               the magnitude of the
             *                                         force, applied to the
             *                                         particle's velocity, in
             *                                         the direction of its
             *                                         global best.
             * @param chi                              the velocity scaling
             *                                         factor.
             * @param vCoeff                           the velocity
             *                                         coefficient.
             * @param leaderSelectionRange             the leader selection
             *                                         range parameter.
             * @param diveristyMehcanism               the diversity mechanism
             *                                         used to maintain
             *                                         diversity on the Pareto
             *                                         front.
             * @param memory                           the memory parameter.
             *******************************************************************/
            NSPSO_Solver(const Instance & instance,
                         unsigned seed,
                         double timeLimit,
                         unsigned maxNumSolutions,
                         bool captureEnabled,
                         unsigned populationSize,
                         unsigned initialIndividualsMethod = 0,
                         double initialIndividualsPercentage = 0.0,
                         double initialIndividualsTimePercentage = 0.0,
                         double omega = 0.6,
                         double c1 = 2.0,
                         double c2 = 2.0,
                         double chi = 1.0,
                         double vCoeff = 0.5,
                         unsigned leaderSelectionRange = 60,
                         std::string diversityMechanism = "crowding distance",
                         bool memory = true);

            /*******************************************************************
             * Constructs a new solver.
             *
             * @param instance        the instance to be solved.
             * @param seed            the seed for the pseudo-random numbers
             *                        generator.
             * @param timeLimit       the time limit in seconds.
             * @param maxNumSolutions the maximum number of solutions.
             * @param captureEnabled  the flag indicating whether the capturing
             *                        of solutions for evaluation is enabled.
             *******************************************************************/
            NSPSO_Solver(const Instance & instance,
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
            NSPSO_Solver(const Instance & instance);

            /******************************
             * Constructs an empty solver.
             ******************************/
            NSPSO_Solver();

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

            /*****************************************************************
             * Sets the percentage of the time limit used to generate the
             * initial individuals.
             *
             * @param initialIndividualsTimePercentage the new percentage of the
             *        time limit used to generate the initial individuals.
             *****************************************************************/
            void setInitialIndividualsTimePercentage(
                    double initialIndividualsTimePercentage);

            /*****************************************
             * Returns the particles' inertia weight.
             *
             * @return the particles' inertia weight.
             *****************************************/
            double getOmega() const;

            /**************************************************
             * Sets the particles' inertia weight.
             *
             * @param omega the new particles' inertia weight.
             **************************************************/
            void setOmega(double omega);

            /****************************************************************
             * Returns the magnitude of the force, applied to the particle's
             * velocity, in the direction of its previous best position.
             *
             * @return the magnitude of the force, applied to the particle's
             *         velocity, in the direction of its previous best
             *         position.
             ****************************************************************/
            double getC1() const;

            /***************************************************************
             * Sets the magnitude of the force, applied to the particle's
             * velocity, in the direction of its previous best position.
             *
             * @param c1 the new magnitude of the force, applied to the
             *        particle's velocity, in the direction of its previous
             *        best position.
             ***************************************************************/
            void setC1(double c1);

            /****************************************************************
             * Returns the magnitude of the force, applied to the particle's
             * velocity, in the direction of its global best (i.e., leader).
             *
             * @return the magnitude of the force, applied to the particle's
             *         velocity, in the direction of its global best 
             *         (i.e., leader).
             ****************************************************************/
            double getC2() const;

            /******************************************************************
             * Sets the magnitude of the force, applied to the particle's
             * velocity, in the direction of its global best (i.e., leader).
             * 
             *
             * @param c2 the new magnitude of the force, applied to the
             *        particle's velocity, in the direction of its global best
             *        (i.e., leader).
             ******************************************************************/
            void setC2(double c2);

            /***************************************
             * Returns the velocity scaling factor.
             *
             * @return the velocity scaling factor.
             ***************************************/
            double getChi() const;

            /**********************************************
             * Sets the velocity scaling factor.
             *
             * @param chi the new velocity scaling factor.
             **********************************************/
            void setChi(double chi);

            /************************************
             * Returns the velocity coefficient.
             *
             * @return the velocity coefficient.
             ************************************/
            double getVCoeff() const;

            /**********************************************
             * Sets the velocity coefficient.
             *
             * @param vCoeff the new velocity coefficient.
             **********************************************/
            void setVCoeff(double vCoeff);

            /**************************************
             * Returns the leader selection range.
             *
             * @return the leader selection range.
             **************************************/
            unsigned getLeaderSelectionRange() const;

            /**************************************************************
             * Sets the leader selection range.
             *
             * @param leaderSelectionRange the new leader selection range.
             **************************************************************/
            void setLeaderSelectionRange(unsigned leaderSelectionRange);

            /****************************************************************
             * Returns the diversity mechanism used to maintain diversity on 
             * the Pareto front.
             *
             * @return the diversity mechanism used to maintain diversity on 
             *         the Pareto front.
             ****************************************************************/
            std::string getDiversityMechanism() const;

            /*****************************************************************
             * Sets the diversity mechanism used to maintain diversity on the
             * Pareto front.
             *
             * @param diversityMehcanism the new mechanism used to maintain
             *        diversity on the Pareto front.
             *****************************************************************/
            void setDiversityMechanism(std::string diversityMechanism);

            /***************************************************************
             * Returns the memory parameter.
             *
             * @return true if the memory is activated in the algorithm for
             *         multiple calls; false otherwise.
             ***************************************************************/
            bool getMemory() const;

            /******************************************
             * Sets the memory parameter.
             *
             * @param memory the new memory parameter.
             ******************************************/
            void setMemory(bool memory);

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

