#pragma once

#define NSBRKGA_MULTIPLE_INCLUSIONS

#include "nsbrkga.hpp"
#include <istream>
#include <ostream>
#include <vector>

namespace motsp {
/***************************************************
 * The Instance class represents an instance of the
 * Multi-Objective Travelling Salesman Problem.
 ***************************************************/
class Instance {
    public:
    /****************************
     * The number of objectives.
     ****************************/
    unsigned num_objectives;

    /**************************
     * The number of vertices.
     **************************/
    unsigned num_vertices;

    /**********************************************************
     * The vertices coordinates.
     **********************************************************/
    std::vector<std::vector<std::pair<double, double>>> coord;

    /**************************************************
     * The adjacency matrices.
     **************************************************/
    std::vector<std::vector<std::vector<double>>> adj;

    /*********************************
     * The optimization senses.
     *********************************/
    std::vector<NSBRKGA::Sense> senses;

    /********************************
     * This instance primal bounds.
     ********************************/
    std::vector<double> primal_bound;

    private:
    /******************************
     * Initializes a new instance.
     ******************************/
    void init();

    public:
    /********************************************************************
     * Constructs a new instance.
     *
     * @param adj the adjacency matrices.
     ********************************************************************/
    Instance(const std::vector<std::vector<std::vector<double>>> & adj);

    /****************************************************************************
     * Constructs a new instance.
     *
     * @param coord the coordinates of the vertices.
     ****************************************************************************/
    Instance(const std::vector<std::vector<std::pair<double, double>>> & coord);

    /********************************
     * Constructs an empty instance.
     ********************************/
    Instance();

    /***********************************************************
     * Verifies whether this instance is valid.
     *
     * @return true if this instance is valid; false otherwise.
     ***********************************************************/
    bool is_valid() const;

    /**************************************************************************
     * Standard input operator.
     *
     * @param is       standard input stream object.
     * @param instance the instance.
     *
     * @return the stream object.
     **************************************************************************/
    friend std::istream & operator >>(std::istream & is, Instance & instance);

    /*************************************************************
     * Standard output operator.
     *
     * @param os       standard output stream object.
     * @param instance the instance.
     *
     * @return the stream object.
     *************************************************************/
    friend std::ostream & operator <<(std::ostream & os,
                                      const Instance & instance);
};

}
