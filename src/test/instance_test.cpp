#include "instance/instance.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

int main() {
    motsp::Instance instance;
    std::ifstream ifs;
    for(const std::string & filename : {"instances/dABC1291.txt",
                                        "instances/dAB1655.txt",
                                        "instances/flABC1400.txt",
                                        "instances/flAB1577.txt",
                                        "instances/kroAB100.txt",
                                        "instances/kroAB150.txt",
                                        "instances/kroAB200.txt",
                                        "instances/kroABC100.txt",
                                        "instances/kroABCD100.txt",
                                        "instances/kroABCDE100.txt",
                                        "instances/kroABCE100.txt",
                                        "instances/kroABD100.txt",
                                        "instances/kroABDE100.txt",
                                        "instances/kroABE100.txt",
                                        "instances/kroAC100.txt",
                                        "instances/kroACD100.txt",
                                        "instances/kroACDE100.txt",
                                        "instances/kroACE100.txt",
                                        "instances/kroAD100.txt",
                                        "instances/kroADE100.txt",
                                        "instances/kroAE100.txt",
                                        "instances/kroBC100.txt",
                                        "instances/kroBCD100.txt",
                                        "instances/kroBCDE100.txt",
                                        "instances/kroBCE100.txt",
                                        "instances/kroBD100.txt",
                                        "instances/kroBDE100.txt",
                                        "instances/kroBE100.txt",
                                        "instances/kroCD100.txt",
                                        "instances/kroCDE100.txt",
                                        "instances/kroCE100.txt",
                                        "instances/kroDE100.txt",
                                        "instances/pcbAB1173.txt",
                                        "instances/rlABCDEF1304.txt",
                                        "instances/rlABCDE1323.txt",
                                        "instances/rlABCD1889.txt",
                                        "instances/rlABC5915.txt",
                                        "instances/rlAB5934.txt",
                                        "instances/uABCDE1060.txt",
                                        "instances/uABCD1432.txt",
                                        "instances/uABC1817.txt",
                                        "instances/uAB2152.txt",
                                        "instances/vmAB1084.txt"}) {
        std::cout << filename << std::endl;

        ifs.open(filename);

        assert(ifs.is_open());

        ifs >> instance;

        ifs.close();

        assert(instance.num_objectives >= 2);
        assert(instance.num_objectives <= 6);
        assert(instance.num_vertices >= 100);
        assert(instance.num_vertices <= 5934);
        assert(instance.is_valid());
    }

    std::cout << std::endl << "Instance Test PASSED" << std::endl;

    return 0;
}

