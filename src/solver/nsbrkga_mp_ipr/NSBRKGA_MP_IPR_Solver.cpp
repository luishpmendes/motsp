#include "NSBRKGA_MP_IPR_Solver.hpp"
#include "Decoder.hpp"
#include "../weighted_sum/christofides/Christofides_Solver.hpp"
#include "../weighted_sum/branch-and-cut/BnC_Solver.hpp"

namespace MOTSP {
    NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver(
            const Instance & instance,
            unsigned seed,
            double timeLimit,
            unsigned maxNumSolutions,
            bool captureEnabled,
            unsigned populationSize,
            unsigned initialIndividualsMethod,
            double initialIndividualsPercentage,
            double initialIndividualsTimePercentage,
            double elitePercentage,
            double mutantPercentage,
            unsigned numTotalParents,
            unsigned numEliteParents,
            BRKGA::BiasFunctionType biasType,
            unsigned numPopulations,
            double prPercentagePairs,
            double prMinDist,
            BRKGA::PathRelinking::Selection prSelection,
            double prPercentage,
            unsigned prInterval,
            unsigned shakeInterval,
            unsigned resetInterval,
            unsigned numThreads,
            unsigned maxLocalSearchIterations) 
        : Solver::Solver(instance,
                         seed,
                         timeLimit,
                         maxNumSolutions,
                         captureEnabled),
          populationSize(populationSize),
          biasType(biasType),
          prSelection(prSelection),
          prInterval(prInterval),
          shakeInterval(shakeInterval),
          resetInterval(resetInterval),
          maxLocalSearchIterations(maxLocalSearchIterations) {
        this->setInitialIndividualsMethod(initialIndividualsMethod);
        this->setInitialIndividualsPercentage(initialIndividualsPercentage);
        this->setInitialIndividualsTimePercentage(
                initialIndividualsTimePercentage);
        this->setElitePercentage(elitePercentage);
        this->setMutantPercentage(mutantPercentage);
        this->setNumTotalParents(numTotalParents);
        this->setNumEliteParents(numEliteParents);
        this->setNumPopulations(numPopulations);
        this->setPrPercentagePairs(prPercentagePairs);
        this->setPrMinDist(prMinDist);
        this->setPrPercentage(prPercentage);
        this->setNumThreads(numThreads);
    }

    NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver(const Instance & instance,
                                                 unsigned seed,
                                                 double timeLimit,
                                                 unsigned maxNumSolutions,
                                                 bool captureEnabled) 
        : NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver(
                instance,
                seed,
                timeLimit,
                maxNumSolutions,
                captureEnabled,
                2 * instance.getNumVertices()) {}

    NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver(const Instance & instance) 
        : NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver(
                instance,
                std::chrono::system_clock::now().time_since_epoch().count()) {}

    NSBRKGA_MP_IPR_Solver::NSBRKGA_MP_IPR_Solver() : Solver::Solver() {}

    unsigned NSBRKGA_MP_IPR_Solver::getPopulationSize() const {
        return this->populationSize;
    }

    void NSBRKGA_MP_IPR_Solver::setPopulationSize(unsigned populationSize) {
        this->populationSize = populationSize;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getInitialIndividualsMethod() const {
        return this->initialIndividualsMethod;
    }

    void NSBRKGA_MP_IPR_Solver::setInitialIndividualsMethod(
            unsigned initialIndividualsMethod) {
        if(initialIndividualsMethod > 2) {
            this->initialIndividualsMethod = 0;
        } else {
            this->initialIndividualsMethod = initialIndividualsMethod;
        }
    }

    double NSBRKGA_MP_IPR_Solver::getInitialIndividualsPercentage() const {
        return this->initialIndividualsPercentage;
    }

    void NSBRKGA_MP_IPR_Solver::setInitialIndividualsPercentage(
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

    double NSBRKGA_MP_IPR_Solver::getInitialIndividualsTimePercentage() const {
        return this->initialIndividualsTimePercentage;
    }

    void NSBRKGA_MP_IPR_Solver::setInitialIndividualsTimePercentage(
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

    double NSBRKGA_MP_IPR_Solver::getElitePercentage() const {
        return this->elitePercentage;
    }

    void NSBRKGA_MP_IPR_Solver::setElitePercentage(double elitePercentage) {
        if(elitePercentage < std::numeric_limits<double>::epsilon()) {
            this->elitePercentage = 0.01;
        } else if(elitePercentage > 1.00) {
            this->elitePercentage = 1.00;
        } else {
            this->elitePercentage = elitePercentage;
        }
    }

    double NSBRKGA_MP_IPR_Solver::getMutantPercentage() const {
        return this->mutantPercentage;
    }

    void NSBRKGA_MP_IPR_Solver::setMutantPercentage(double mutantPercentage) {
        if(mutantPercentage < 0.00) {
            this->mutantPercentage = 0.00;
        } else if(mutantPercentage > 1.00) {
            this->mutantPercentage = 1.00;
        } else {
            this->mutantPercentage = mutantPercentage;
        }
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumTotalParents() const {
        return this->numTotalParents;
    }

    void NSBRKGA_MP_IPR_Solver::setNumTotalParents(unsigned numTotalParents) {
        if(numTotalParents < 2) {
            this->numTotalParents = 2;
        } else if(numTotalParents > this->populationSize) {
            this->numTotalParents = this->populationSize;
        } else {
            this->numTotalParents = numTotalParents;
        }
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumEliteParents() const {
        return this->numEliteParents;
    }

    void NSBRKGA_MP_IPR_Solver::setNumEliteParents(unsigned numEliteParents) {
        if(numEliteParents < 1) {
            this->numEliteParents = 1;
        } else if(numEliteParents > this->numTotalParents) {
            this->numEliteParents = this->numTotalParents;
        } else {
            this->numEliteParents = numEliteParents;
        }
    }

    BRKGA::BiasFunctionType NSBRKGA_MP_IPR_Solver::getBiasType() const {
        return this->biasType;
    }

    void NSBRKGA_MP_IPR_Solver::setBiasType(
            const BRKGA::BiasFunctionType & biasType) {
        this->biasType = biasType;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumPopulations() const {
        return this->numPopulations;
    }

    void NSBRKGA_MP_IPR_Solver::setNumPopulations(unsigned numPopulations) {
        if(numPopulations < 1) {
            this->numPopulations = 1;
        } else {
            this->numPopulations = numPopulations;
        }
    }

    double NSBRKGA_MP_IPR_Solver::getPrPercentagePairs() const {
        return this->prPercentagePairs;
    }

    void NSBRKGA_MP_IPR_Solver::setPrPercentagePairs(double prPercentagePairs) {
        if(prPercentagePairs < 0.00) {
            this->prPercentagePairs = 0.00;
        } else if(prPercentagePairs > 1.00) {
            this->prPercentagePairs = 1.00;
        } else {
            this->prPercentagePairs = prPercentagePairs;
        }
    }

    double NSBRKGA_MP_IPR_Solver::getPrMinDist() const {
        return this->prMinDist;
    }

    void NSBRKGA_MP_IPR_Solver::setPrMinDist(double prMinDist) {
        if(prMinDist < 0.0) {
            this->prMinDist = 0.0;
        } else {
            this->prMinDist = prMinDist;
        }
    }

    BRKGA::PathRelinking::Selection NSBRKGA_MP_IPR_Solver::getPrSelection() const {
        return this->prSelection;
    }

    void NSBRKGA_MP_IPR_Solver::setPrSelection(
            BRKGA::PathRelinking::Selection prSelection) {
        this->prSelection = prSelection;
    }

    double NSBRKGA_MP_IPR_Solver::getPrPercentage() const {
        return this->prPercentage;
    }

    void NSBRKGA_MP_IPR_Solver::setPrPercentage(double prPercentage) {
        if(prPercentage < std::numeric_limits<double>::epsilon()) {
            this->prPercentage = 0.01;
        } else if(prPercentage > 1.00) {
            this->prPercentage = 1.00;
        } else {
            this->prPercentage = prPercentage;
        }
    }

    unsigned NSBRKGA_MP_IPR_Solver::getPrInterval() const {
        return this->prInterval;
    }

    void NSBRKGA_MP_IPR_Solver::setPrInterval(unsigned prInterval) {
        this->prInterval = prInterval;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getShakeInterval() const {
        return this->shakeInterval;
    }

    void NSBRKGA_MP_IPR_Solver::setShakeInterval(unsigned shakeInterval) {
        this->shakeInterval = shakeInterval;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getResetInterval() const {
        return this->resetInterval;
    }

    void NSBRKGA_MP_IPR_Solver::setResetInterval(unsigned resetInterval) {
        this->resetInterval = resetInterval;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumThreads() const {
        return this->numThreads;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getMaxLocalSearchIterations() const {
        return this->maxLocalSearchIterations;
    }

    void NSBRKGA_MP_IPR_Solver::setMaxLocalSearchIterations(
            unsigned maxLocalSearchIterations) {
        this->maxLocalSearchIterations = maxLocalSearchIterations;
    }

    void NSBRKGA_MP_IPR_Solver::setNumThreads(unsigned numThreads) {
        if(numThreads < 1) {
            this->numThreads = 1;
        } else {
            this->numThreads = numThreads;
        }
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumGenerations() const {
        return this->numGenerations;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getLastUpdateGeneration() const {
        return this->lastUpdateGeneration;
    }

    double NSBRKGA_MP_IPR_Solver::getLastUpdateTime() const {
        return this->lastUpdateTime;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getLargeOffset() const {
        return this->largeOffset;
    }

    double NSBRKGA_MP_IPR_Solver::getPathRelinkTime() const {
        return this->pathRelinkTime;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumPathRelinkCalls() const {
        return this->numPathRelinkCalls;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumHomogeneities() const {
        return this->numHomogeneities;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumEliteImprovments() const {
        return this->numEliteImprovments;
    }

    unsigned NSBRKGA_MP_IPR_Solver::getNumBestImprovments() const {
        return this->numBestImprovements;
    }

    void NSBRKGA_MP_IPR_Solver::solve() {
        this->startTime = std::chrono::steady_clock::now();

        Decoder decoder(this->instance,
                        this->numThreads,
                        this->maxLocalSearchIterations);

        this->senses =
            std::vector<BRKGA::Sense>(this->instance.getNumObjectives(),
                                      BRKGA::Sense::MINIMIZE);

        BRKGA::BrkgaParams params;
        params.num_incumbent_solutions = this->maxNumSolutions;
        params.population_size = this->populationSize;
        params.elite_percentage = this->elitePercentage;
        params.mutants_percentage = this->mutantPercentage;
        params.total_parents = this->numTotalParents;
        params.num_elite_parents = this->numEliteParents;
        params.bias_type = this->biasType;
        params.num_independent_populations = this->numPopulations;
        params.pr_number_pairs = this->prPercentagePairs *
            (this->maxNumSolutions * this->elitePercentage) *
            (this->maxNumSolutions * this->elitePercentage);
        params.pr_minimum_distance = this->prMinDist;
        params.pr_type = BRKGA::PathRelinking::Type::PERMUTATION;
        params.pr_selection = this->prSelection;
        params.alpha_block_size = 0.1;
        params.pr_percentage = this->prPercentage;

        BRKGA::NSBRKGA_MP_IPR algorithm(decoder,
                                        this->senses,
                                        this->seed,
                                        this->instance.getNumVertices() - 1,
                                        params,
                                        this->numThreads);

        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            initialIndividuals;
        unsigned initialIndividualsIterations = 0;
        std::vector<std::vector<BRKGA::Chromosome>> initialPopulations(
                this->numPopulations);
        std::vector<std::vector<std::vector<double>>> initialFitnesses(
                this->numPopulations);
        std::vector<std::vector<double>> pareto;

        if(this->initialIndividualsMethod == 1) {
            Christofides_Solver solver(
                    this->instance,
                    this->seed,
                    this->timeLimit * this->initialIndividualsTimePercentage,
                    this->initialIndividualsPercentage *
                        ((double) this->numPopulations * this->populationSize),
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
                        ((double) this->numPopulations * this->populationSize),
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

        if(!initialIndividuals.empty()) {
            for(unsigned i = 0; i < initialIndividuals.size(); i++) {
                initialPopulations[i % this->numPopulations].push_back(
                        initialIndividuals[i].second);
            }

            algorithm.setInitialPopulations(initialPopulations);
        }

        algorithm.initialize();

        std::vector<std::pair<std::vector<double>, std::vector<double>>>
            newIndividuals = algorithm.getIncumbentSolutions();

        this->updateIndividuals(newIndividuals);

        std::shared_ptr<BRKGA::DistanceFunctionBase> dist_func(
                new BRKGA::KendallTauDistance());

        this->numGenerations = 0;
        this->lastUpdateGeneration = 0;
        this->largeOffset = 0;

        this->pathRelinkTime = 0.0;
        this->numPathRelinkCalls = 0;
        this->numHomogeneities = 0;
        this->numBestImprovements = 0;
        this->numEliteImprovments = 0;
        this->numShakings = 0;
        this->numResets = 0;

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

        std::uniform_int_distribution<int> distribution(
                1,
                this->instance.getNumVertices() - 1);

        while(!this->areTerminationCriteriaMet()) {
            this->numGenerations++;

            if(algorithm.evolve()) {
                this->lastUpdateTime = this->getElapsedTime();

                auto updateOffset = this->numGenerations -
                                    this->lastUpdateGeneration;
                this->lastUpdateGeneration = this->numGenerations;

                if(this->largeOffset < updateOffset) {
                    this->largeOffset = updateOffset;
                }

                newIndividuals = algorithm.getIncumbentSolutions();

                if(this->updateIndividuals(newIndividuals) && this->captureEnabled) {
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

            unsigned generationsWithoutImprovement = this->numGenerations -
                this->lastUpdateGeneration;

            if(this->largeOffset < generationsWithoutImprovement) {
                this->largeOffset = generationsWithoutImprovement;
            }

            if(this->prInterval > 0 && generationsWithoutImprovement > 0 &&
                    (generationsWithoutImprovement % this->prInterval == 0)) {
                this->numPathRelinkCalls++;
                const auto prStartTime = std::chrono::steady_clock::now();
                auto result = algorithm.pathRelink(
                        params.pr_type, 
                        params.pr_selection,
                        dist_func, 
                        params.pr_number_pairs, 
                        params.pr_minimum_distance, 
                        1, 
                        this->timeLimit - this->getElapsedTime(), 
                        params.pr_percentage);

                const auto prTime = Solver::getElapsedTime(prStartTime);
                this->pathRelinkTime += prTime;

                switch(result) {
                    case BRKGA::PathRelinking::PathRelinkingResult::TOO_HOMOGENEOUS: {
                        this->numHomogeneities++;
                        break;
                    }
                    case BRKGA::PathRelinking::PathRelinkingResult::ELITE_IMPROVEMENT: {
                        this->numEliteImprovments++;
                    }
                    case BRKGA::PathRelinking::PathRelinkingResult::BEST_IMPROVEMENT: {
                        this->numBestImprovements++;
                        this->lastUpdateTime = this->getElapsedTime();

                        auto updateOffset = this->numGenerations -
                                            this->lastUpdateGeneration;
                        this->lastUpdateGeneration = this->numGenerations;

                        if(this->largeOffset < updateOffset) {
                            this->largeOffset = updateOffset;
                        }

                        newIndividuals = algorithm.getIncumbentSolutions();

                        if(this->updateIndividuals(newIndividuals) &&
                                this->captureEnabled) {
                            std::vector<std::vector<double>> pareto(
                                    this->individuals.size());
                            std::transform(this->individuals.begin(),
                                           this->individuals.end(),
                                           pareto.begin(),
                                           [](const auto & individual) {
                                                return individual.first;
                                           });
                            this->capturedParetos.push_back(pareto);
                            this->capturedTimes.push_back(
                                    this->getElapsedTime());
                            this->capturedIterations.push_back(
                                    this->numGenerations + 
                                    initialIndividualsIterations);
                        }

                        break;
                    }

                    default: {
                        break;
                    }
                }
            }

            if(this->shakeInterval > 0 
                    && generationsWithoutImprovement > 0 
                    && (generationsWithoutImprovement % this->shakeInterval 
                        == 0)) {
                this->numShakings++;
                unsigned intensity = distribution(this->rng);
                algorithm.shake(intensity, BRKGA::ShakingType::SWAP);
            }

            if(this->resetInterval > 0 
                    && generationsWithoutImprovement > 0 
                    && (generationsWithoutImprovement % this->resetInterval) 
                        == 0) {
                this->numResets++;
                algorithm.reset();
                if(!initialIndividuals.empty()) {
                    shuffle(initialIndividuals.begin(), 
                            initialIndividuals.end(),
                            this->rng);
                    initialPopulations.clear();
                    initialPopulations.resize(this->numPopulations);
                    initialFitnesses.clear();
                    initialFitnesses.resize(this->numPopulations);

                    for(unsigned i = 0; i < initialIndividuals.size(); i++) {
                        initialPopulations[i % this->numPopulations].push_back(
                                initialIndividuals[i].second);
                        initialFitnesses[i % this->numPopulations].push_back(
                                initialIndividuals[i].first);
                    }

                    for(unsigned i = 0; i < this->numPopulations; i++) {
                        for(unsigned j = 0;
                            j < initialPopulations[i].size();
                            j++) {
                            algorithm.injectChromosome(
                                    initialPopulations[i][j],
                                    i,
                                    j,
                                    initialFitnesses[i][j]);
                        }
                    }
                }
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

    void NSBRKGA_MP_IPR_Solver::write(std::ostream & os) const {
        Solver::write(os);
        os << "Number of individuals in each population: "
           << this->populationSize << std::endl
           << "Initial solutions method: " << this->initialIndividualsMethod
           << std::endl
           << "Initial solutions time percentage: "
           << this->initialIndividualsTimePercentage << std::endl
           << "Percentage of individuals to become the elite set: " 
           << this->elitePercentage << std::endl
           << "Percentage of mutants to be inserted in the population: " 
           << this->mutantPercentage << std::endl
           << "Number of total parents for mating: " << this->numTotalParents 
           << std::endl
           << "Number of elite parents for mating: " << this->numEliteParents 
           << std::endl
           << "Type of bias that will be used: " << this->biasType << std::endl
           << "Number of independent parallel populations: "
           << this->numPopulations << std::endl
           << "Percentage of pairs of chromosomes to be tested to path-relinking: " 
           << this->prPercentagePairs << std::endl
           << "Minimum distance between chromosomes selected to path-relinking: " 
           << this->prMinDist << std::endl
           << "Individual selection to path-relinking: " << this->prSelection 
           << std::endl
           << "Percentage of the path to be computed: " << this->prPercentage 
           << std::endl
           << "Interval at which the path relink is applied: "
           << this->prInterval << std::endl
           << "Interval at which the populations are shaken: "
           << this->shakeInterval << std::endl
           << "Interval at which the populations are reset: " 
           << this->resetInterval << std::endl
           << "Number of threads to be used during parallel decoding: " 
           << this->numThreads << std::endl
           << "The number of generations evolved: " << this->numGenerations 
           << std::endl
           << "Last update generation: " << this->lastUpdateGeneration 
           << std::endl
           << "Last update time: " << this->lastUpdateTime << std::endl
           << "Largest number of generations between improvements: " 
           << this->largeOffset << std::endl
           << "Total path relink time: " << this->pathRelinkTime << std::endl
           << "Total path relink calls: " << this->numPathRelinkCalls 
           << std::endl
           << "Number of homogeneities: " << this->numHomogeneities 
           << std::endl
           << "Improvements in the elite set: " << this->numEliteImprovments 
           << std::endl
           << "Best individual improvements: " << this->numBestImprovements 
           << std::endl
           << "Total shakings calls: " << this->numShakings << std::endl
           << "Total resets calls: " << this->numResets << std::endl;
    }

    void NSBRKGA_MP_IPR_Solver::write(const char * filename) const {
        std::ofstream ofs;
        ofs.open(filename);

        if (ofs.is_open()) {
            this->write(ofs);
            ofs.close();
        } else {
            throw "File not created.";
        }
    }

    void NSBRKGA_MP_IPR_Solver::write(const std::string & filename) const {
        this->write(filename.c_str());
    }

    void NSBRKGA_MP_IPR_Solver::write() const {
        this->write(std::cout);
    }
}

