#include "NSPSO_Solver.hpp"
#include "Problem.hpp"
#include "../weighted_sum/christofides/Christofides_Solver.hpp"
#include "../weighted_sum/branch-and-cut/BnC_Solver.hpp"
#include <pagmo/algorithms/nspso.hpp>

namespace MOTSP {
    NSPSO_Solver::NSPSO_Solver(const Instance & instance,
                               unsigned seed,
                               double timeLimit,
                               unsigned maxNumSolutions,
                               bool captureEnabled,
                               unsigned populationSize,
                               unsigned initialIndividualsMethod,
                               double initialIndividualsPercentage,
                               double initialIndividualsTimePercentage,
                               double omega,
                               double c1,
                               double c2,
                               double chi,
                               double vCoeff,
                               unsigned leaderSelectionRange,
                               std::string diversityMechanism,
                               bool memory)
        : Solver::Solver(instance,
                         seed,
                         timeLimit,
                         maxNumSolutions,
                         captureEnabled),
          populationSize(populationSize),
          memory(memory) {
        this->setInitialIndividualsMethod(initialIndividualsMethod);
        this->setInitialIndividualsPercentage(initialIndividualsPercentage);
        this->setInitialIndividualsTimePercentage(
                initialIndividualsTimePercentage);
        this->setOmega(omega);
        this->setC1(c1);
        this->setC2(c2);
        this->setChi(chi);
        this->setVCoeff(vCoeff);
        this->setLeaderSelectionRange(leaderSelectionRange);
        this->setDiversityMechanism(diversityMechanism);
    }

    NSPSO_Solver::NSPSO_Solver(const Instance & instance,
                               unsigned seed,
                               double timeLimit,
                               unsigned maxNumSolutions,
                               bool captureEnabled) 
        : NSPSO_Solver::NSPSO_Solver(instance,
                                     seed,
                                     timeLimit,
                                     maxNumSolutions,
                                     captureEnabled,
                                     2 * instance.getNumVertices()) {}

    NSPSO_Solver::NSPSO_Solver(const Instance & instance)
        : NSPSO_Solver::NSPSO_Solver(
                instance,
                std::chrono::system_clock::now().time_since_epoch().count()) {}

    NSPSO_Solver::NSPSO_Solver() : Solver::Solver() {}

    bool NSPSO_Solver::updateIndividuals(const pagmo::population & pop) {
        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            individuals(pop.size());

        for(unsigned i = 0; i < pop.size(); i++) {
            individuals[i] = std::make_pair(pop.get_f()[i], pop.get_x()[i]);
        }

        return Solver::updateIndividuals(individuals);
    }

    unsigned NSPSO_Solver::getPopulationSize() const {
        return this->populationSize;
    }

    void NSPSO_Solver::setPopulationSize(unsigned populationSize) {
        this->populationSize = populationSize;
    }

    unsigned NSPSO_Solver::getInitialIndividualsMethod() const {
        return this->initialIndividualsMethod;
    }

    void NSPSO_Solver::setInitialIndividualsMethod(
            unsigned initialIndividualsMethod) {
        if(initialIndividualsMethod > 2) {
            this->initialIndividualsMethod = 0;
        } else {
            this->initialIndividualsMethod = initialIndividualsMethod;
        }
    }

    double NSPSO_Solver::getInitialIndividualsPercentage() const {
        return this->initialIndividualsPercentage;
    }

    void NSPSO_Solver::setInitialIndividualsPercentage(
            double initialIndividualsPercentage) {
        if(initialIndividualsPercentage < 0.0) {
            this->initialIndividualsPercentage = 0.0;
        } else if(initialIndividualsPercentage > 0.5) {
            this->initialIndividualsPercentage = 0.5;
        } else {
            this->initialIndividualsPercentage =
                initialIndividualsPercentage;
        }
    }

    double NSPSO_Solver::getInitialIndividualsTimePercentage() const {
        return this->initialIndividualsTimePercentage;
    }

    void NSPSO_Solver::setInitialIndividualsTimePercentage(
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

    double NSPSO_Solver::getOmega() const {
        return this->omega;
    }

    void NSPSO_Solver::setOmega(double omega) {
        if(omega < 0.0) {
            this->omega = 0.0;
        } else if(omega > 1.0) {
            this->omega = 1.0;
        } else {
            this->omega = omega;
        }
    }

    double NSPSO_Solver::getC1() const {
        return this->c1;
    }

    void NSPSO_Solver::setC1(double c1) {
        if(c1 <= 0.00) {
            this->c1 = 0.01;
        } else {
            this->c1 = c1;
        }
    }

    double NSPSO_Solver::getC2() const {
        return this->c2;
    }

    void NSPSO_Solver::setC2(double c2) {
        if(c2 <= 0.00) {
            this->c2 = 0.01;
        } else {
            this->c2 = c2;
        }
    }

    double NSPSO_Solver::getChi() const {
        return this->chi;
    }

    void NSPSO_Solver::setChi(double chi) {
        if(chi <= 0.00) {
            this->chi = 0.01;
        } else {
            this->chi = chi;
        }
    }

    double NSPSO_Solver::getVCoeff() const {
        return this->vCoeff;
    }

    void NSPSO_Solver::setVCoeff(double vCoeff) {
        if(vCoeff <= 0.00) {
            this->vCoeff = 0.01;
        } else if(vCoeff > 1.00) {
            this->vCoeff = 1.00;
        } else {
            this->vCoeff = vCoeff;
        }
    }

    unsigned NSPSO_Solver::getLeaderSelectionRange() const {
        return this->leaderSelectionRange;
    }

    void NSPSO_Solver::setLeaderSelectionRange(unsigned leaderSelectionRange) {
        if(leaderSelectionRange <= 0) {
            this->leaderSelectionRange = 1;
        } else if(leaderSelectionRange > 100) {
            this->leaderSelectionRange = 100;
        } else {
            this->leaderSelectionRange = leaderSelectionRange;
        }
    }

    std::string NSPSO_Solver::getDiversityMechanism() const {
        return this->diversityMechanism;
    }

    void NSPSO_Solver::setDiversityMechanism(std::string diversityMechanism) {
        if(diversityMechanism == "crowding distance" 
                || diversityMechanism == "niche count" 
                || diversityMechanism == "max min") {
            this->diversityMechanism = diversityMechanism;
        }
    }

    bool NSPSO_Solver::getMemory() const {
        return this->memory;
    }

    void NSPSO_Solver::setMemory(bool memory) {
        this->memory = memory;
    }

    unsigned NSPSO_Solver::getNumGenerations() const {
        return this->numGenerations;
    }

    void NSPSO_Solver::solve() {
        this->startTime = std::chrono::steady_clock::now();

        pagmo::problem prob{Problem(this->instance)};

        pagmo::algorithm algo{pagmo::nspso(1, 
                                           this->omega,
                                           this->c1,
                                           this->c2,
                                           this->chi,
                                           this->vCoeff,
                                           this->leaderSelectionRange,
                                           this->diversityMechanism,
                                           this->memory,
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
            initialIndividuals = solver.getIndividuals();

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
                           [](const auto & individual) {
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

    void NSPSO_Solver::write(std::ostream & os) const {
        Solver::write(os);
        os << "Population size: " << this->populationSize << std::endl
           << "Initial solutions method: " << this->initialIndividualsMethod
           << std::endl
           << "Initial solutions time percentage: "
           << this->initialIndividualsTimePercentage << std::endl
           << "Omega: " << this->omega << std::endl
           << "C1: " << this->c1 << std::endl
           << "C2: " << this->c2 << std::endl
           << "Chi: "  << this->chi << std::endl
           << "vCoeff: " << this->vCoeff << std::endl
           << "Leader selection range: " << this->leaderSelectionRange 
           << std::endl
           << "Diversity mechanism: " << this->diversityMechanism << std::endl
           << "Memory: " << this->memory << std::endl
           << "Number of generations: " << this->numGenerations << std::endl;
    }

    void NSPSO_Solver::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if (ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void NSPSO_Solver::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void NSPSO_Solver::write() const {
        this->write(std::cout);
    }
}

