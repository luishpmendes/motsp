#include "NSGA2_Solver.hpp"
#include "Problem.hpp"
#include "../weighted_sum/christofides/Christofides_Solver.hpp"
#include "../weighted_sum/branch-and-cut/BnC_Solver.hpp"
#include <pagmo/algorithms/nsga2.hpp>

namespace MOTSP {
    NSGA2_Solver::NSGA2_Solver(const Instance & instance,
                               unsigned seed,
                               double timeLimit,
                               unsigned maxNumSolutions,
                               bool captureEnabled,
                               unsigned populationSize,
                               unsigned initialIndividualsMethod,
                               double initialIndividualsPercentage,
                               double initialIndividualsTimePercentage,
                               double crossoverProbability,
                               double crossoverDistribution,
                               double mutationProbability,
                               double mutationDistribution)
        : Solver::Solver(instance,
                         seed,
                         timeLimit,
                         maxNumSolutions,
                         captureEnabled) {
        this->setInitialIndividualsMethod(initialIndividualsMethod);
        this->setInitialIndividualsPercentage(initialIndividualsPercentage);
        this->setInitialIndividualsTimePercentage(
                initialIndividualsTimePercentage);
        this->setCrossoverProbability(crossoverProbability);
        this->setCrossoverDistribution(crossoverDistribution);
        this->setMutationProbability(mutationProbability);
        this->setMutationDistribution(mutationDistribution);

        this->populationSize = round((double(populationSize)/4.0))*4;

        this->rng.discard(1000);
    }

    NSGA2_Solver::NSGA2_Solver(const Instance & instance,
                               unsigned seed,
                               double timeLimit,
                               unsigned maxNumSolutions,
                               bool captureEnabled)
        : NSGA2_Solver::NSGA2_Solver(instance,
                                     seed,
                                     timeLimit,
                                     maxNumSolutions,
                                     captureEnabled,
                                     2 * instance.getNumVertices()) {}

    NSGA2_Solver::NSGA2_Solver(const Instance & instance)
        : NSGA2_Solver::NSGA2_Solver(
                instance,
                std::chrono::system_clock::now().time_since_epoch().count()) {}

    NSGA2_Solver::NSGA2_Solver() : Solver::Solver() {}


    bool NSGA2_Solver::updateIndividuals(const pagmo::population & pop) {
        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            newIndividuals(pop.size());

        for(unsigned i = 0; i < pop.size(); i++) {
            newIndividuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
        }

        return Solver::updateIndividuals(newIndividuals);
    }

    unsigned NSGA2_Solver::getPopulationSize() const {
        return this->populationSize;
    }

    void NSGA2_Solver::setPopulationSize(unsigned populationSize) {
        this->populationSize = populationSize;
    }

    unsigned NSGA2_Solver::getInitialIndividualsMethod() const {
        return this->initialIndividualsMethod;
    }

    void NSGA2_Solver::setInitialIndividualsMethod(
            unsigned initialIndividualsMethod) {
        if(initialIndividualsMethod > 2) {
            this->initialIndividualsMethod = 0;
        } else {
            this->initialIndividualsMethod = initialIndividualsMethod;
        }
    }

    double NSGA2_Solver::getInitialIndividualsPercentage() const {
        return this->initialIndividualsPercentage;
    }

    void NSGA2_Solver::setInitialIndividualsPercentage(
            double initialIndividualsPercentage) {
        if(initialIndividualsPercentage < 0.0) {
            this->initialIndividualsPercentage = 0.0;
        } else if(initialIndividualsPercentage > 0.5) {
            this->initialIndividualsPercentage = 0.5;
        } else {
            this->initialIndividualsPercentage = initialIndividualsPercentage;
        }
    }

    double NSGA2_Solver::getInitialIndividualsTimePercentage() const {
        return this->initialIndividualsTimePercentage;
    }

    void NSGA2_Solver::setInitialIndividualsTimePercentage(
            double initialIndividualsTimePercentage) {
        if(initialIndividualsTimePercentage < 0.0) {
            this->initialIndividualsTimePercentage = 0.0;
        } else if(initialIndividualsTimePercentage > 0.5) {
            this->initialIndividualsTimePercentage = 0.5;
        } else {
            this->initialIndividualsTimePercentage =
                initialIndividualsTimePercentage;
        }
    }

    double NSGA2_Solver::getCrossoverProbability() const {
        return this->crossoverProbability;
    }

    void NSGA2_Solver::setCrossoverProbability(double crossoverProbability) {
        if(crossoverProbability < 0.00) {
            this->crossoverProbability = 0.00;
        } else if(crossoverProbability >= 1.00) {
            this->crossoverProbability = 0.99;
        } else {
            this->crossoverProbability = crossoverProbability;
        }
    }

    double NSGA2_Solver::getCrossoverDistribution() const {
        return this->crossoverDistribution;
    }

    void NSGA2_Solver::setCrossoverDistribution(double crossoverDistribution) {
        if(crossoverDistribution < 1.00) {
            this->crossoverDistribution = 1.00;
        } else if(crossoverDistribution >= 100.00) {
            this->crossoverDistribution = 99.99;
        } else {
            this->crossoverDistribution = crossoverDistribution;
        }
    }

    double NSGA2_Solver::getMutationProbability() const {
        return this->mutationProbability;
    }

    void NSGA2_Solver::setMutationProbability(double mutationProbability) {
        if(mutationProbability < 0.00) {
            this->mutationProbability = 0.00;
        } else if(mutationProbability > 1.00) {
            this->mutationProbability = 1.00;
        } else {
            this->mutationProbability = mutationProbability;
        }
    }

    double NSGA2_Solver::getMutationDistribution() const {
        return this->mutationDistribution;
    }

    void NSGA2_Solver::setMutationDistribution(double mutationDistribution) {
        if(mutationDistribution < 1.00) {
            this->mutationDistribution = 1.00;
        } else if(mutationDistribution >= 100.00) {
            this->mutationDistribution = 99.99;
        } else {
            this->mutationDistribution = mutationDistribution;
        }
    }

    unsigned NSGA2_Solver::getNumGenerations() const {
        return this->numGenerations;
    }

    void NSGA2_Solver::solve() {
        this->startTime = std::chrono::steady_clock::now();

        pagmo::problem prob{Problem(this->instance)};

        pagmo::algorithm algo{pagmo::nsga2(1, 
                                           this->crossoverProbability,
                                           this->crossoverDistribution,
                                           this->mutationProbability,
                                           this->mutationDistribution,
                                           this->seed)};

        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            initialIndividuals;
        unsigned initialIndividualsIterations = 0;

        std::vector<std::vector<double>> pareto;

        if(this->initialIndividualsMethod == 1) {
            Christofides_Solver solver(
                    this->instance,
                    this->seed,
                    this->timeLimit * this->initialIndividualsTimePercentage,
                    this->initialIndividualsPercentage * 
                        ((double) this->populationSize),
                    this->captureEnabled);

            solver.solve();
            initialIndividuals = solver.getAllIndividuals();

            if(this->captureEnabled && 
                    !solver.getCapturedIterations().empty()) {
                initialIndividualsIterations =
                    solver.getCapturedIterations().back();
            }
        } else if(this->initialIndividualsMethod == 2) {
            BnC_Solver solver(
                    this->instance,
                    this->seed,
                    this->timeLimit * this->initialIndividualsTimePercentage,
                    this->initialIndividualsPercentage * 
                        ((double) this->populationSize),
                    this->captureEnabled,
                    1);

            solver.solve();
            initialIndividuals = solver.getAllIndividuals();

            if(this->captureEnabled && 
                    !solver.getCapturedIterations().empty()) {
                initialIndividualsIterations =
                    solver.getCapturedIterations().back();
            }
        }

        pagmo::population pop{
            prob, 
            this->populationSize - initialIndividuals.size(),
            this->seed};

        for(const auto & individual : initialIndividuals) {
            pop.push_back(individual.second, individual.first);
        }

        this->updateIndividuals(pop);

        this->numGenerations = 0;

        if(this->captureEnabled) {
            pareto.resize(this->individuals.size());
            std::transform(this->individuals.begin(),
                           this->individuals.end(),
                           pareto.begin(),
                           [](const auto & individual) {
                                return individual.first;
                           });
            this->capturedParetos.push_back(pareto);
            this->capturedTimes.push_back(this->getElapsedTime());
            this->capturedIterations.push_back(
                    this->numGenerations + initialIndividualsIterations);
        }

        while(!this->areTerminationCriteriaMet()) {
            this->numGenerations++;
            pop = algo.evolve(pop);

            if(this->updateIndividuals(pop) && this->captureEnabled) {
                pareto.resize(this->individuals.size());
                std::transform(this->individuals.begin(),
                               this->individuals.end(),
                               pareto.begin(),
                               [](const auto & individual) {
                                    return individual.first;
                               });
                this->capturedParetos.push_back(pareto);
                this->capturedTimes.push_back(this->getElapsedTime());
                this->capturedIterations.push_back(
                        this->numGenerations + initialIndividualsIterations);
            }
        }

        if(this->captureEnabled) {
            pareto.resize(this->individuals.size());
            std::transform(this->individuals.begin(),
                           this->individuals.end(),
                           pareto.begin(),
                           [](const auto & individual){
                                return individual.first;
                           });
            this->capturedParetos.push_back(pareto);
            this->capturedTimes.push_back(this->getElapsedTime());
            this->capturedIterations.push_back(
                    this->numGenerations + initialIndividualsIterations);
        }

        this->solutions.resize(this->individuals.size());

        std::transform(this->individuals.begin(),
                       this->individuals.end(),
                       this->solutions.begin(),
                       [](const auto & individual) {
                            return Solution(individual.second,
                                            individual.first);
                       }
                );

        this->solvingTime = this->getElapsedTime();
    }

    void NSGA2_Solver::write(std::ostream & os) const {
        Solver::write(os);
        os << "Population size: " << this->populationSize << std::endl
           << "Initial solutions method: " << this->initialIndividualsMethod
           << std::endl
           << "Initial solutions time percentage: "
           << this->initialIndividualsTimePercentage << std::endl
           << "Crossover probability: " << this->crossoverProbability 
           << std::endl
           << "Distribution index for crossover: " 
           << this->crossoverDistribution << std::endl
           << "Mutation probability: " << this->mutationProbability 
           << std::endl
           << "Distribution index for mutation: " 
           << this->mutationDistribution << std::endl
           << "Number of generations: " << this->numGenerations << std::endl;
    }

    void NSGA2_Solver::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if (ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void NSGA2_Solver::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void NSGA2_Solver::write() const {
        this->write(std::cout);
    }
}

