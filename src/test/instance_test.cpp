#include "instance/instance.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

int main() {
    motsp::Instance instance;
    std::ifstream ifs;
    for(const std::string filename : {"instances/kroAB100.txt",
                                      "instances/kroAC100.txt",
                                      "instances/kroAD100.txt",
                                      "instances/kroAE100.txt",
                                      "instances/kroBC100.txt",
                                      "instances/kroBD100.txt",
                                      "instances/kroBE100.txt",
                                      "instances/kroCD100.txt",
                                      "instances/kroCE100.txt",
                                      "instances/kroDE100.txt",
                                      "instances/kroAB150.txt",
                                      "instances/kroAB200.txt",
                                      "instances/kroAB300.txt",
                                      "instances/kroAB400.txt",
                                      "instances/kroAB500.txt",
                                      "instances/kroAB750.txt",
                                      "instances/kroAB1000.txt",
                                      "instances/kroABC100.txt",
                                      "instances/kroABCD100.txt",
                                      "instances/kroABC150.txt",
                                      "instances/kroABCD150.txt",
                                      "instances/kroABC200.txt",
                                      "instances/kroABCD200.txt",
                                      "instances/kroABC300.txt",
                                      "instances/kroABCD300.txt",
                                      "instances/kroABC400.txt",
                                      "instances/kroABCD400.txt",
                                      "instances/kroABC500.txt",
                                      "instances/kroABCD500.txt",
                                      "instances/kroABC750.txt",
                                      "instances/kroABCD750.txt",
                                      "instances/kroABC1000.txt",
                                      "instances/kroABCD1000.txt",
                                     }) {
        std::cout << filename << std::endl;

        ifs.open(filename);

        assert(ifs.is_open());

        ifs >> instance;

        ifs.close();

        assert(instance.num_objectives >= 2);
        assert(instance.num_objectives <= 4);
        assert(instance.num_vertices >= 100);
        assert(instance.num_vertices <= 1000);
        assert(instance.is_valid());
    }

    std::cout << std::endl << "Instance Test PASSED" << std::endl;

    return 0;
}
