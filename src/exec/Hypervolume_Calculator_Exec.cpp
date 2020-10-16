#include "../instance/Instance.hpp"
#include "../utils/ArgumentParser.hpp"
#include <pagmo/utils/hypervolume.hpp>
#include <fstream>

int main(int argc, char * argv[]) {
    ArgumentParser argParser (argc, argv);

    if(argParser.cmdOptionExists("--instance")) {
        std::string instanceFilename = argParser.getCmdOption("--instance");
        MOTSP::Instance instance(instanceFilename);
        unsigned numObjectives = instance.getNumObjectives();
        std::vector<std::vector<std::vector<double>>> paretos;
        std::vector<std::vector<unsigned>> capturedIterations;
        std::vector<std::vector<double>> capturedTimes;
        std::vector<std::vector<std::vector<std::vector<double>>>>
            capturedParetos;
        std::vector<double> minCosts(numObjectives,
                                     std::numeric_limits<double>::max()),
                            maxCosts(numObjectives,
                                     std::numeric_limits<double>::lowest());
        std::vector<double> referencePoint(numObjectives, 1.0);
        unsigned numSolvers;

        for(numSolvers = 0;
            argParser.cmdOptionExists("--pareto-" + std::to_string(numSolvers));
            numSolvers++) {}

        paretos.resize(numSolvers);
        capturedIterations.resize(numSolvers);
        capturedTimes.resize(numSolvers);
        capturedParetos.resize(numSolvers);

        for(unsigned i = 0; i < numSolvers; i++) {
            if(!argParser.cmdOptionExists("--pareto-" + std::to_string(i))) {
                break;
            }

            std::string paretoFilename = argParser.getCmdOption(
                    "--pareto-" + std::to_string(i));
            std::ifstream ifs;
            ifs.open(paretoFilename);

            if(ifs.is_open()) {
                for(std::string line; std::getline(ifs, line);) {
                    std::istringstream iss(line);
                    std::vector<double> costs(numObjectives, 0.0);

                    for(unsigned j = 0; j < numObjectives; j++) {
                        if(!(iss >> costs[j])) {
                            throw "Error reading file.";
                        }

                        if(minCosts[j] > costs[j]) {
                            minCosts[j] = costs[j];
                        }

                        if(maxCosts[j] < costs[j]) {
                            maxCosts[j] = costs[j];
                        }
                    }

                    paretos[i].push_back(costs);
                }
            } else {
                throw "File not found.";
            }
        }

        for(unsigned i = 0; i < numSolvers; i++) {
            if(!argParser.cmdOptionExists(
                        "--captured-paretos-" + std::to_string(i))) {
                break;
            }

            std::string capturedParetosFilename = argParser.getCmdOption(
                    "--captured-paretos-" + std::to_string(i));

            for(unsigned j = 0; ; j++) {
                std::ifstream ifs;
                ifs.open(capturedParetosFilename + std::to_string(j) + ".txt");

                if(ifs.is_open()) {
                    unsigned capturedIteration;
                    double capturedTime;

                    ifs >> capturedIteration >> capturedTime;

                    capturedIterations[i].push_back(capturedIteration);
                    capturedTimes[i].push_back(capturedTime);
                    capturedParetos[i].emplace_back();

                    ifs.ignore();

                    for(std::string line; std::getline(ifs, line);) {
                        std::istringstream iss(line);
                        std::vector<double> costs(numObjectives, 0.0);

                        for(unsigned j = 0; j < numObjectives; j++) {
                            if(!(iss >> costs[j])) {
                                throw "Error reading file.";
                            }

                            if(minCosts[j] > costs[j]) {
                                minCosts[j] = costs[j];
                            }

                            if(maxCosts[j] < costs[j]) {
                                maxCosts[j] = costs[j];
                            }
                        }

                        capturedParetos[i].back().push_back(costs);
                    }
                } else {
                    break;
                }
            }
        }

        for(unsigned i = 0; i < numSolvers; i++) {
            if(!argParser.cmdOptionExists(
                        "--hypervolume-" + std::to_string(i))) {
                break;
            }

            std::string hypervolumeFilename =
                argParser.getCmdOption("--hypervolume-" + std::to_string(i));

            std::ofstream ofs;
            ofs.open(hypervolumeFilename);

            if(ofs.is_open()) {
                std::vector<std::vector<double>> normalizedPareto(
                        paretos[i].size());
                for(unsigned j = 0; j < paretos[i].size(); j++) {
                    normalizedPareto[j] = 
                        std::vector<double>(paretos[i][j].size(), 0.0);
                    for(unsigned k = 0; k < numObjectives; k++) {
                        normalizedPareto[j][k] = 
                            (paretos[i][j][k] - minCosts[k])
                            /(maxCosts[k] - minCosts[k]);
                    }
                }

                pagmo::hypervolume hv(normalizedPareto);
                double hypervolume = hv.compute(referencePoint);
                ofs << hypervolume << std::endl;
            } else {
                throw "File not created.";
            }
        }

        for(unsigned i = 0; i < numSolvers; i++) {
            std::string capturedHypervolumesFilename = argParser.getCmdOption(
                    "--captured-hypervolumes-" + std::to_string(i));

            std::ofstream ofs;
            ofs.open(capturedHypervolumesFilename);

            if(ofs.is_open()) {
                std::vector<std::vector<std::vector<double>>>
                    capturedNormalizedParetos(capturedParetos[i].size());
                for(unsigned j = 0;
                    j < capturedParetos[i].size();
                    j++) {
                    capturedNormalizedParetos[j] = 
                        std::vector<std::vector<double>>(
                                capturedParetos[i][j].size());
                    for(unsigned k = 0; k < capturedParetos[i][j].size(); k++) {
                        capturedNormalizedParetos[j][k] = std::vector<double>(
                                capturedParetos[i][j][k].size(), 0.0);
                        for(unsigned l = 0; l < numObjectives; l++) {
                            capturedNormalizedParetos[j][k][l] = 
                                (capturedParetos[i][j][k][l] -
                                 minCosts[l])
                                /(maxCosts[l] - minCosts[l]);
                        }
                    }

                    pagmo::hypervolume hv(capturedNormalizedParetos[j]);
                    double capturedHypervolume = hv.compute(referencePoint);
                    ofs << capturedIterations[i][j] << ","
                        << capturedTimes[i][j] << "," 
                        << capturedHypervolume << std::endl;
                }
            } else {
                throw "File not created.";
            }
        }
    } else {
        std::cerr << "./HyperVolume_Calculator_Exec "
                  << "--instance <instanceFilename> "
                  << "--pareto-i <paretoFilename> "
                  << "--captured-paretos-i <capturedParetosFilename> "
                  << "--hypervolume-i <hypervolumeFilename> "
                  << "--captured-hypervolumes-i <capturedHypervolumesFilename> "
                  << std::endl;
    }

    return 0;
}

