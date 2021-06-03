#include "solver/nsmpbrkga/nsmpbrkga_solver.hpp"

namespace motsp {

NSMPBRKGA_Solver::NSMPBRKGA_Solver(const Instance & instance)
    : Solver::Solver(instance) {}

void NSMPBRKGA_Solver::capture_snapshot(
        const BRKGA::NSMPBRKGA<Decoder> & algorithm) {
    this->pareto.resize(this->best_individuals.size());
    std::transform(this->best_individuals.begin(),
                   this->best_individuals.end(),
                   this->pareto.begin(),
                   [](const auto & individual) {
                        return individual.first;
                   });
    this->pareto_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                     this->elapsed_time(),
                                                     this->pareto));

    this->num_non_dominated.resize(this->num_populations);
    this->num_fronts.resize(this->num_populations);
    this->num_elites.resize(this->num_populations);
    this->num_mutants.resize(this->num_populations);

    for(unsigned i = 0; i < this->num_populations; i++) {
        this->num_non_dominated[i] =
            algorithm.getCurrentPopulation(i).num_non_dominated;
        this->num_fronts[i] = algorithm.getCurrentPopulation(i).num_fronts;
        this->num_elites[i] = algorithm.getCurrentPopulation(i).num_elites;
        this->num_mutants[i] = algorithm.getCurrentPopulation(i).num_mutants;
    }

    this->non_dominated_snapshots.push_back(std::make_tuple(
                this->num_iterations,
                this->elapsed_time(),
                this->num_non_dominated));

    this->fronts_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                     this->elapsed_time(),
                                                     this->num_fronts));

    this->num_elites_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                         this->elapsed_time(),
                                                         this->num_elites));

    this->num_mutants_snapshots.push_back(std::make_tuple(this->num_iterations,
                                                          this->elapsed_time(),
                                                          this->num_mutants));

    this->time_last_snapshot = this->elapsed_time();
    this->iteration_last_snapshot = this->num_iterations;
    this->num_snapshots++;
}

void NSMPBRKGA_Solver::solve() {
    this->start_time = std::chrono::steady_clock::now();

    Decoder decoder(this->instance,
                    this->num_threads,
                    this->max_local_search_iterations);

    this->senses = std::vector<BRKGA::Sense>(this->instance.num_objectives,
                                             BRKGA::Sense::MINIMIZE);

    BRKGA::BrkgaParams params;
    params.num_incumbent_solutions = this->max_num_solutions;
    params.population_size = this->population_size;
    params.min_elites_percentage = this->min_elites_percentage;
    params.max_elites_percentage = this->max_elites_percentage;
    params.min_mutants_percentage = this->min_mutants_percentage;
    params.max_mutants_percentage = this->max_mutants_percentage;
    params.total_parents = this->num_total_parents;
    params.num_elite_parents = this->num_elite_parents;
    params.bias_type = this->bias_type;
    params.diversity_type = this->diversity_type;
    params.num_independent_populations = this->num_populations;
    params.pr_number_pairs = this->pr_number_pairs;
    params.pr_minimum_distance = this->pr_min_dist;
    params.pr_type = BRKGA::PathRelinking::Type::PERMUTATION;
    params.pr_selection = this->pr_selection;
    params.alpha_block_size = 0.1;
    params.pr_percentage = this->pr_percentage;

    BRKGA::NSMPBRKGA algorithm(decoder,
                               this->senses,
                               this->seed,
                               this->instance.num_vertices - 1,
                               params,
                               this->num_threads);

    std::vector<std::vector<BRKGA::Chromosome>> initial_populations(
            this->num_populations);
    std::vector<std::vector<std::vector<double>>> initial_fitnesses(
            this->num_populations);

    if(!this->initial_individuals.empty()) {
        for(unsigned i = 0; i < this->initial_individuals.size(); i++) {
            initial_populations[i % this->num_populations].push_back(
                    this->initial_individuals[i].second);
        }

        algorithm.setInitialPopulations(initial_populations);
    }

    algorithm.initialize();

    this->update_best_individuals(algorithm.getIncumbentSolutions());

    std::shared_ptr<BRKGA::DistanceFunctionBase> dist_func(
            new BRKGA::KendallTauDistance());

    this->num_iterations = 0;
    this->last_update_generation = 0;
    this->large_offset = 0;

    this->path_relink_time = 0.0;
    this->num_path_relink_calls = 0;
    this->num_homogeneities = 0;
    this->num_best_improvements = 0;
    this->num_elite_improvments = 0;
    this->num_shakings = 0;
    this->num_resets = 0;

    if(this->max_num_snapshots > 0) {
        this->time_between_snapshots = this->time_limit /
            this->max_num_snapshots;
        this->iterations_between_snapshots = this->iterations_limit /
            this->max_num_snapshots;
        this->capture_snapshot(algorithm);
    }

    std::uniform_int_distribution<unsigned> distribution(
            1,
            this->instance.num_vertices - 1);

    while(!this->are_termination_criteria_met()) {
        this->num_iterations++;

        if(algorithm.evolve()) {
            this->last_update_time = this->elapsed_time();

            auto update_offset = this->num_iterations -
                                 this->last_update_generation;
            this->last_update_generation = this->num_iterations;

            if(this->large_offset < update_offset) {
                this->large_offset = update_offset;
            }

            this->update_best_individuals(algorithm.getIncumbentSolutions());
        }

        if(this->max_num_snapshots > 0 &&
           this->num_snapshots < this->max_num_snapshots &&
          (this->elapsed_time() - this->time_last_snapshot >=
           this->time_between_snapshots ||
           this->num_iterations - this->iteration_last_snapshot >=
           this->iterations_between_snapshots)) {
            this->capture_snapshot(algorithm);
        }

        unsigned generations_without_improvement = this->num_iterations -
            this->last_update_generation;

        if(this->large_offset < generations_without_improvement) {
            this->large_offset = generations_without_improvement;
        }

        if(this->pr_interval > 0 && generations_without_improvement > 0 &&
                (generations_without_improvement % this->pr_interval == 0)) {
            this->num_path_relink_calls++;
            const auto pr_start_time = std::chrono::steady_clock::now();
            auto result = algorithm.pathRelink(
                    params.pr_type,
                    params.pr_selection,
                    dist_func,
                    params.pr_number_pairs,
                    params.pr_minimum_distance,
                    1,
                    this->time_limit - this->elapsed_time(),
                    params.pr_percentage);

            const auto pr_time = Solver::elapsed_time(pr_start_time);
            this->path_relink_time += pr_time;

            switch(result) {
                case BRKGA::PathRelinking::PathRelinkingResult::TOO_HOMOGENEOUS: {
                    this->num_homogeneities++;
                    break;
                }
                case BRKGA::PathRelinking::PathRelinkingResult::ELITE_IMPROVEMENT: {
                    this->num_elite_improvments++;
                }
                case BRKGA::PathRelinking::PathRelinkingResult::BEST_IMPROVEMENT: {
                    this->num_best_improvements++;
                    this->last_update_time = this->elapsed_time();

                    auto update_offset = this->num_iterations -
                                         this->last_update_generation;
                    this->last_update_generation = this->num_iterations;

                    if(this->large_offset < update_offset) {
                        this->large_offset = update_offset;
                    }

                    this->update_best_individuals(
                            algorithm.getIncumbentSolutions());

                    break;
                }

                default: {
                    break;
                }
            }
        }

        if(this->shake_interval > 0 && generations_without_improvement > 0 &&
                (generations_without_improvement % this->shake_interval == 0)) {
            this->num_shakings++;
            unsigned intensity = distribution(this->rng);
            algorithm.shake(intensity, BRKGA::ShakingType::SWAP);
        }

        if(this->reset_interval > 0 && generations_without_improvement > 0 &&
                (generations_without_improvement % this->reset_interval) == 0) {
            this->num_resets++;
            algorithm.reset();
            if(!this->initial_individuals.empty()) {
                shuffle(this->initial_individuals.begin(),
                        this->initial_individuals.end(),
                        this->rng);
                initial_populations.clear();
                initial_populations.resize(this->num_populations);
                initial_fitnesses.clear();
                initial_fitnesses.resize(this->num_populations);

                for(unsigned i = 0; i < this->initial_individuals.size(); i++) {
                    initial_populations[i % this->num_populations].push_back(
                            this->best_individuals[i].second);
                    initial_fitnesses[i % this->num_populations].push_back(
                            this->best_individuals[i].first);
                }

                for(unsigned i = 0; i < this->num_populations; i++) {
                    for(unsigned j = 0;
                        j < initial_populations[i].size();
                        j++) {
                        algorithm.injectChromosome(initial_populations[i][j],
                                                   i,
                                                   j,
                                                   initial_fitnesses[i][j]);
                    }
                }
            }
        }
    }

    if(this->max_num_snapshots > 0) {
        this->capture_snapshot(algorithm);
    }

    this->best_solutions.clear();

    for(const auto & best_individual : this->best_individuals) {
        this->best_solutions.push_back(Solution(this->instance,
                                                best_individual.second));
    }

    this->solving_time = this->elapsed_time();
}
std::ostream & operator <<(std::ostream & os, const NSMPBRKGA_Solver & solver) {
    os << static_cast<const Solver &>(solver)
       << "Number of individuals in each population: " << solver.population_size
       << std::endl
       << "Minimum percentage of individuals to become the elite set: "
       << solver.min_elites_percentage << std::endl
       << "Maximum percentage of individuals to become the elite set: "
       << solver.max_elites_percentage << std::endl
       << "Minimum percentage of mutants to be inserted in the population: "
       << solver.min_mutants_percentage << std::endl
       << "Maximum percentage of mutants to be inserted in the population: "
       << solver.max_mutants_percentage << std::endl
       << "Number of total parents for mating: " << solver.num_total_parents
       << std::endl
       << "Number of elite parents for mating: " << solver.num_elite_parents
       << std::endl
       << "Type of bias that will be used: "
       << EnumIO<BRKGA::BiasFunctionType>::enum_names().at(
               static_cast<int>(solver.bias_type)) << std::endl
       << "Type of diversity that will be used: "
       << EnumIO<BRKGA::DiversityFunctionType>::enum_names().at(
               static_cast<int>(solver.diversity_type)) << std::endl
       << "Number of independent parallel populations: "
       << solver.num_populations << std::endl
       << "Number of pairs of chromosomes to be tested to path-relinking: "
       << solver.pr_number_pairs << std::endl
       << "Minimum distance between chromosomes selected to path-relinking: "
       << solver.pr_min_dist << std::endl
       << "Individual selection to path-relinking: "
       << EnumIO<BRKGA::PathRelinking::Selection>::enum_names().at(
               static_cast<int>(solver.pr_selection)) << std::endl
       << "Percentage of the path to be computed: " << solver.pr_percentage
       << std::endl
       << "Interval at which the path relink is applied: "
       << solver.pr_interval << std::endl
       << "Interval at which the populations are shaken: "
       << solver.shake_interval << std::endl
       << "Interval at which the populations are reset: "
       << solver.reset_interval << std::endl
       << "Number of threads to be used during parallel decoding: "
       << solver.num_threads << std::endl
       << "Last update generation: " << solver.last_update_generation
       << std::endl
       << "Last update time: " << solver.last_update_time << std::endl
       << "Largest number of generations between improvements: "
       << solver.large_offset << std::endl
       << "Total path relink time: " << solver.path_relink_time << std::endl
       << "Total path relink calls: " << solver.num_path_relink_calls
       << std::endl
       << "Number of homogeneities: " << solver.num_homogeneities << std::endl
       << "Improvements in the elite set: " << solver.num_elite_improvments
       << std::endl
       << "Best individual improvements: " << solver.num_best_improvements
       << std::endl
       << "Total shakings calls: " << solver.num_shakings << std::endl
       << "Total resets calls: " << solver.num_resets << std::endl;
    return os;
}

}

