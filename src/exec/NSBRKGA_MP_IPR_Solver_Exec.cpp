#include "../utils/ArgumentParser.hpp"
#include "../solver/nsbrkga_mp_ipr/NSBRKGA_MP_IPR_Solver.hpp"

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
        unsigned populationSize = 2 * instance.getNumVertices();
        unsigned initialIndividualsMethod = 0;
        double initialIndividualsPercentage = 0.0;
        double initialIndividualsTimePercentage = 0.0;
        double elitePercentage = 0.17;
        double mutantPercentage = 0.19;
        unsigned numTotalParents = 5;
        unsigned numEliteParents = 3;
        BRKGA::BiasFunctionType biasType = BRKGA::BiasFunctionType::LOGINVERSE;
        unsigned numPopulations = 1;
        double prPercentagePairs = 0.75;
        double prMinDist = 0.15;
        BRKGA::PathRelinking::Selection prSelection =
            BRKGA::PathRelinking::Selection::BESTSOLUTION;
        double prPercentage = 0.31;
        unsigned prInterval = 100;
        unsigned shakeInterval = 200;
        unsigned resetInterval = 500;
        unsigned numThreads = 1;
        unsigned maxLocalSearchIterations = 0;

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

        if(argParser.cmdOptionExists("--population-size")) {
            populationSize =
                std::stoul(argParser.getCmdOption("--population-size"));
        }

        if(argParser.cmdOptionExists("--initial-individuals-method")) {
            initialIndividualsMethod =
                std::stoul(
                        argParser.getCmdOption("--initial-individuals-method"));
        }

        if(argParser.cmdOptionExists("--initial-individuals-percentage")) {
            initialIndividualsPercentage =
                std::stod(argParser.getCmdOption(
                            "--initial-individuals-percentage"));
        }

        if(argParser.cmdOptionExists("--initial-individuals-time-percentage")) {
            initialIndividualsTimePercentage =
                std::stod(argParser.getCmdOption(
                            "--initial-individuals-time-percentage"));
        }

        if(argParser.cmdOptionExists("--elite-percentage")) {
            elitePercentage =
                std::stod(argParser.getCmdOption("--elite-percentage"));
        }

        if(argParser.cmdOptionExists("--mutant-percentage")) {
            mutantPercentage =
                std::stod(argParser.getCmdOption("--mutant-percentage"));
        }

        if(argParser.cmdOptionExists("--num-total-parents")) {
            numTotalParents =
                std::stoul(argParser.getCmdOption("--num-total-parents"));
        }

        if(argParser.cmdOptionExists("--num-elite-parents")) {
            numEliteParents =
                std::stoul(argParser.getCmdOption("--num-elite-parents"));
        }

        if(argParser.cmdOptionExists("--bias-type")) {
            std::stringstream ss(argParser.getCmdOption("--bias-type"));
            ss >> biasType;
        }

        if(argParser.cmdOptionExists("--num-populations")) {
            numPopulations =
                std::stoul(argParser.getCmdOption("--num-populations"));
        }

        if(argParser.cmdOptionExists("--pr-percentage-pairs")) {
            prPercentagePairs =
                std::stod(argParser.getCmdOption("--pr-percentage-pairs"));
        }

        if(argParser.cmdOptionExists("--pr-min-dist")) {
            prMinDist = std::stod(argParser.getCmdOption("--pr-min-dist"));
        }

        if(argParser.cmdOptionExists("--pr-selection")) {
            std::stringstream ss(argParser.getCmdOption("--pr-selection"));
            ss >> prSelection;
        }

        if(argParser.cmdOptionExists("--pr-percentage")) {
            prPercentage = std::stod(argParser.getCmdOption("--pr-percentage"));
        }

        if(argParser.cmdOptionExists("--pr-interval")) {
            prInterval = std::stoul(argParser.getCmdOption("--pr-interval"));
        }

        if(argParser.cmdOptionExists("--shake-interval")) {
            shakeInterval =
                std::stoul(argParser.getCmdOption("--shake-interval"));
        }

        if(argParser.cmdOptionExists("--reset-interval")) {
            resetInterval =
                std::stoul(argParser.getCmdOption("--reset-interval"));
        }

        if(argParser.cmdOptionExists("--num-threads")) {
            numThreads = std::stoul(argParser.getCmdOption("--num-threads"));
        }

        if(argParser.cmdOptionExists("--max-local-search-iterations")) {
            maxLocalSearchIterations =
                std::stoul(argParser.getCmdOption("--max-local-search-iterations"));
        }

        MOTSP::NSBRKGA_MP_IPR_Solver solver(instance,
                                            seed,
                                            timeLimit,
                                            maxNumSolutions,
                                            captureEnabled,
                                            populationSize,
                                            initialIndividualsMethod,
                                            initialIndividualsPercentage,
                                            initialIndividualsTimePercentage,
                                            elitePercentage,
                                            mutantPercentage,
                                            numTotalParents,
                                            numEliteParents,
                                            biasType,
                                            numPopulations,
                                            prPercentagePairs,
                                            prMinDist,
                                            prSelection,
                                            prPercentage,
                                            prInterval,
                                            shakeInterval,
                                            resetInterval,
                                            numThreads,
                                            maxLocalSearchIterations);

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
        std::cerr << "./NSBRKGA_MP_IPR_Solver_Exec "
                  << "--instance <instanceFileName> "
                  << "--seed <seed> "
                  << "--time-limit <timeLimit> "
                  << "--max-num-solutions <maxNumSolutions> "
                  << "--capture-enabled "
                  << "--population-size <populationSize> "
                  << "--initial-individuals-method <initialIndividualsMethod> "
                  << "--initial-individuals-percentage <initialIndividualsPercentage> "
                  << "--initial-individuals-time-percentage <initialIndividualsTimePercentage> "
                  << "--elite-percentage <elitePercentage> "
                  << "--mutant-percentage <mutantPercentage> "
                  << "--num-total-parents <numTotalParents> "
                  << "--num-elite-parents <numEliteParents> "
                  << "--bias-type <biasType> "
                  << "--num-populations <numPopulations> "
                  << "--pr-percentage-pairs <prPercentagePairs> "
                  << "--pr-min-dist <prMinDist> "
                  << "--pr-selection <prSelection> "
                  << "--pr-percentage <prPercentage> "
                  << "--pr-interval <prInterval> "
                  << "--shake-interval <shakeInterval> "
                  << "--reset-interval <resetInterval> "
                  << "--num-threads <numThreads> "
                  << "--max-local-search-iterations <maxLocalSearchIterations> "
                  << "--statistics <statisticsFilename> "
                  << "--solutions <solutionsFilename> "
                  << "--pareto <paretoFilename>"
                  << "--captured-paretos <capturedParetosFilename> "
                  << std::endl;
    }

    return 0;
}

