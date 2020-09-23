#include "../utils/ArgumentParser.hpp"
#include "../solver/weighted_sum/christofides/Christofides_Solver.hpp"

int main (int argc, char * argv[]) {
    ArgumentParser argParser (argc, argv);

    if(argParser.cmdOptionExists("--instance")) {
        std::string instanceFilename = argParser.getCmdOption("--instance");
        MOTSP::Instance instance(instanceFilename);
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();
        double timeLimit = 3600;
        unsigned maxNumSolutions = std::numeric_limits<unsigned>::max();
        bool captureEnabled = false;

        if(argParser.cmdOptionExists("--seed")) {
            seed = std::stoul(argParser.getCmdOption("--seed"));
        }

        if(argParser.cmdOptionExists("--time-limit")) {
            timeLimit = std::stod(argParser.getCmdOption("--time-limit"));
        }

        if(argParser.cmdOptionExists("--max-num-solutions")) {
            maxNumSolutions =
                std::stoul(argParser.getCmdOption("--max-num-solutions"));
        }

        captureEnabled = argParser.cmdOptionExists("--capture-enabled");

        MOTSP::Christofides_Solver solver(instance,
                                          seed,
                                          timeLimit,
                                          maxNumSolutions,
                                          captureEnabled);

        solver.solve();

        if(argParser.cmdOptionExists("--statistics")) {
            solver.write(argParser.getCmdOption("--statistics"));
        }

        if(argParser.cmdOptionExists("--solutions")) {
            std::string solutionFilename =
                argParser.getCmdOption("--solutions");
            std::vector<MOTSP::Solution> solutions = solver.getSolutions();

            for(unsigned i = 0; i < solutions.size(); i++) {
                solutions[i].write(
                        solutionFilename + std::to_string(i) + ".sol");
            }
        }

        if(argParser.cmdOptionExists("--pareto")) {
            std::string paretoFilename = argParser.getCmdOption("--pareto");

            std::ofstream ofs;
            ofs.open(paretoFilename);

            if(ofs.is_open()) {
                std::vector<MOTSP::Solution> solutions = solver.getSolutions();

                for(unsigned i = 0; i < solutions.size(); i++) {
                    for(unsigned j = 0; 
                        j < solutions[i].getCost().size() - 1; 
                        j++) {
                        ofs << solutions[i].getCost()[j] << " ";
                    }

                    ofs << solutions[i].getCost().back() << std::endl;
                }
            } else {
                throw "File not created.";
            }
        }

        if(argParser.cmdOptionExists("--captured-paretos")) {
            std::string capturedParetosFilename =
                argParser.getCmdOption("--captured-paretos");
            std::vector<std::vector<std::vector<double>>> capturedParetos =
                solver.getCapturedParetos();
            std::vector<double> capturedTimes = solver.getCapturedTimes();
            std::vector<unsigned> capturedIterations =
                solver.getCapturedIterations();

            for(unsigned i = 0; i < capturedParetos.size(); i++) {
                std::ofstream ofs;
                ofs.open(capturedParetosFilename + std::to_string(i) + ".txt");

                if(ofs.is_open()) {
                    ofs << capturedIterations[i] << " " 
                        << capturedTimes[i] << std::endl;
                    for(unsigned j = 0; j < capturedParetos[i].size(); j++) {
                        for(unsigned k = 0; 
                            k < capturedParetos[i][j].size() - 1;
                            k++) {
                            ofs << capturedParetos[i][j][k] << " ";
                        }

                        ofs << capturedParetos[i][j].back() << std::endl;
                    }
                } else {
                    throw "File not created.";
                }
            }
        }
    } else {
        std::cerr << "./Christofides_Solver_Exec "
                  << "--instance <instanceFilename> "
                  << "--seed <seed> "
                  << "--time-limit <timeLimit> "
                  << "--max-num-solutions <maxNumSolutions> "
                  << "--capture-enabled "
                  << "--statistics <statisticsFilename> "
                  << "--solutions <solutionsFilename> "
                  << "--pareto <paretoFilename> "
                  << "--captured-paretos <capturedParetosFilename> "
                  << std::endl;
    }

    return 0;
}

