#pragma once

#include "../../Solver.hpp"

namespace MOTSP {
    class Christofides_Solver : public Solver {
        private:
            /****************************************************************
             * All individuals found during optimisation.
             ****************************************************************/
            std::vector<std::pair<std::vector<double>, std::vector<double>>>
                allIndividuals;

        public:
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
            Christofides_Solver(const Instance & instance,
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
            Christofides_Solver(const Instance & instance);

            /******************************
             * Constructs an empty solver.
             ******************************/
            Christofides_Solver();

            /****************************************************************
             * Returns all individuals found during optimisation.
             *
             * @return all individuals found during optimisation.
             ****************************************************************/
            std::vector<std::pair<std::vector<double>, std::vector<double>>>
                getAllIndividuals() const;

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

