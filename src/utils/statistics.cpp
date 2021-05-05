#include "utils/statistics.hpp"
#include <cmath>

Statistics::Statistics() : size(0.0),
                           mean(0.0),
                           variance(0.0),
                           standard_deviation(0.0) {}


void Statistics::insert(double value) {
    double old_mean = this->mean;
    this->size++;
    this->mean += (value - old_mean) / this->size;
    this->variance += ((value - old_mean) * (value - this->mean) - 
                      this->variance) / this->size;
    this->standard_deviation = std::sqrt(this->variance);
}

