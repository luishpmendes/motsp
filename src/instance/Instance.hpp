#pragma once

#include <string>
#include <vector>

namespace MOTSP {
    /***************************************************
     * The Instance class represents an instance of the 
     * Multi-Objective Travelling Salesman Problem.
     ***************************************************/
    class Instance {
        private:
            /****************************
             * The number of objectives.
             ****************************/
            unsigned numObjectives;

            /**************************
             * The number of vertices.
             **************************/
            unsigned numVertices;

            /**********************************************************
             * The vertices coordinates.
             **********************************************************/
            std::vector<std::vector<std::pair<double, double>>> coord;

            /**************************************************
             * The adjacency matrices.
             **************************************************/
            std::vector<std::vector<std::vector<double>>> adj;

            /******************************
             * Initializes a new instance.
             ******************************/
            void init();

            /*******************************************
             * Initializes a new instance.
             *
             * @param is the input stream to read from.
             *******************************************/
            void init(std::istream & is);

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

            /*******************************************
             * Constructs a new instance.
             *
             * @param is the input stream to read from.
             *******************************************/
            Instance(std::istream & is);

            /*****************************************
             * Constructs a new instance.
             *
             * @param filename the file to read from.
             *****************************************/
            Instance(const char * filename);

            /*****************************************
             * Constructs a new instance.
             *
             * @param filename the file to read from.
             *****************************************/
            Instance(const std::string & filename);

            /********************************
             * Constructs an empty instance.
             ********************************/
            Instance();

            /************************************
             * Returns the number of objectives.
             *
             * @return the number of objectives.
             ************************************/
            unsigned getNumObjectives() const;

            /**********************************
             * Returns the number of vertices.
             *
             * @return the number of vertices.
             **********************************/
            unsigned getNumVertices() const;

            /*****************************************************************************
             * Returns the vertices coordinates.
             *
             * @return the vertices coordinates.
             *****************************************************************************/
            const std::vector<std::vector<std::pair<double, double>>> & getCoord() const;

            /*********************************************************************
             * Returns the adjacency matrices.
             *
             * @return the adjacency matrices.
             *********************************************************************/
            const std::vector<std::vector<std::vector<double>>> & getAdj() const;

            /***********************************************************
             * Verifies whether this instance is valid.
             *
             * @return true if this instance is valid; false otherwise.
             ***********************************************************/
            bool isValid() const;

            /********************************************************
             * Write this instance into the specified output stream.
             *
             * @param os the output stream to write into.
             ********************************************************/
            void write(std::ostream & os) const;

            /***********************************************
             * Write this instance into the specified file.
             *
             * @param filename the file to write into.
             ***********************************************/
            void write(const char * filename) const;

            /***********************************************
             * Write this instance into the specified file.
             *
             * @param filename the file to write into.
             ***********************************************/
            void write(const std::string & filename) const;

            /*******************************************************
             * Write this instance into the standard output stream.
             *******************************************************/
            void write() const;
    };
}

