#include "utils/statistics.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int main () {
    Statistics statistics;

    assert(statistics.size == 0);
    assert(fabs(statistics.mean) < 0.001);
    assert(fabs(statistics.variance) < 0.001);
    assert(fabs(statistics.standard_deviation) < 0.001);

    statistics.insert(89.0);

    assert(statistics.size == 1);
    assert(fabs(statistics.mean - 89.0) < 0.001);
    assert(fabs(statistics.variance) < 0.001);
    assert(fabs(statistics.standard_deviation) < 0.001);

    statistics.insert(13.0);

    assert(statistics.size == 2);
    assert(fabs(statistics.mean - 51.0) < 0.001);
    assert(fabs(statistics.variance - 1444.0) < 0.001);
    assert(fabs(statistics.standard_deviation - 38.0) < 0.001);

    statistics.insert(5.0);

    assert(statistics.size == 3);
    assert(fabs(statistics.mean - 35.66667) < 0.001);
    assert(fabs(statistics.variance - 1432.88889) < 0.001);
    assert(fabs(statistics.standard_deviation - 37.85352) < 0.001);

    statistics.insert(3.0);

    assert(statistics.size == 4);
    assert(fabs(statistics.mean - 27.5) < 0.001);
    assert(fabs(statistics.variance - 1274.75) < 0.001);
    assert(fabs(statistics.standard_deviation - 35.70364) < 0.001);

    statistics.insert(2.0);

    assert(statistics.size == 5);
    assert(fabs(statistics.mean - 22.4) < 0.001);
    assert(fabs(statistics.variance - 1123.84) < 0.001);
    assert(fabs(statistics.standard_deviation - 33.52372) < 0.001);

    std::cout << "Statistics Test PASSED" << std::endl;

    return 0;
}

