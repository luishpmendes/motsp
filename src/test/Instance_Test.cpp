#include "../instance/Instance.hpp"
#include <cassert>
#include <iostream>

int main() {
    MOTSP::Instance instance;

    instance = MOTSP::Instance("instances/kroAB100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroAC100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroAD100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroAE100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBC100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBD100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBE100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroCD100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroCE100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroDE100.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroAB150.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 150);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroAB200.txt");
    assert(instance.getNumObjectives() == 2);
    assert(instance.getNumVertices() == 200);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABC100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABD100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroACD100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroACE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroADE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBCD100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBCE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBDE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroCDE100.txt");
    assert(instance.getNumObjectives() == 3);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABCD100.txt");
    assert(instance.getNumObjectives() == 4);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABCE100.txt");
    assert(instance.getNumObjectives() == 4);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABDE100.txt");
    assert(instance.getNumObjectives() == 4);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroACDE100.txt");
    assert(instance.getNumObjectives() == 4);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroBCDE100.txt");
    assert(instance.getNumObjectives() == 4);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    instance = MOTSP::Instance("instances/kroABCDE100.txt");
    assert(instance.getNumObjectives() == 5);
    assert(instance.getNumVertices() == 100);
    assert(instance.isValid());

    std::cout << "Instance Test PASSED" << std::endl;

    return 0;
}

