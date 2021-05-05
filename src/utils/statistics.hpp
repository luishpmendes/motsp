#pragma once

/*******************************************************
 * The Statistics class represents a data structure for 
 * keeping track of statistics of a sequence of values.
 *******************************************************/
class Statistics {
    public:
    /**********************************
     * This statistics' sequence size.
     **********************************/
    unsigned int size;

    /**********************************
     * This statistics' sequence mean.
     **********************************/
    double mean;

    /**************************************
     * This statistics' sequence variance.
     **************************************/
    double variance;

    /************************************************
     * This statistics' sequence standard deviation.
     ************************************************/
    double standard_deviation;

    /***********************************************************************
     * Creates a new data structure for statistics of a sequence of values.
     ***********************************************************************/
    Statistics();

    /****************************************************************************
     * Inserts a new value into this statistics' sequence.
     *
     * @param value the new value to be inserted into this statistics' sequence.
     ****************************************************************************/
    void insert(double value);
};

