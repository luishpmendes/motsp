#pragma once

#include <limits>
#include <vector>

namespace MOTSP {
    /**************************************************************************
     * The TwoOpt static class implements the 2-opt local search heuristic for
     * the Multi-Objective Travelling Salesman Problem.
     **************************************************************************/
    class TwoOpt {
        public:
            /*******************************************************************
             * Improves the specified solution by applying the 2-opt local
             * search heuristic.
             *
             * @param adj                the adjacency matrices of the instance
             *                           been solved.
             * @param cycle              the cycle of the solution to be
             *                           improved.
             * @param maxNumImprovements the maximum number of improvements.
             * @param maxNumSolutions    the maximum number of solutions to be
             *                           kept during the search
             *
             * @return the improved solutions obtained by applying the 2-opt
             *         local search heuristic.
             *******************************************************************/
            static std::vector<std::vector<unsigned>> improve(
                    const std::vector<std::vector<std::vector<double>>> & adj,
                    const std::vector<unsigned> & cycle,
                    unsigned maxNumImprovments = 
                        std::numeric_limits<unsigned>::max(),
                    unsigned maxNumSolutions =
                        std::numeric_limits<unsigned>::max());
    };
}

