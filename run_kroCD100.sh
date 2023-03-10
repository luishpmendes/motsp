#!/bin/bash

instances=(kroCD100)
solvers=(nsga2 nspso moead mhaco ihs nsmpbrkga)
seeds=(2351389233 840853377 405525247 583443166 76615162 145906556 237064314 77957907 834621397 633844880 598092365)

time_limit=3600
population_size=1000
max_num_solutions=2500
max_num_snapshots=60

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p best_solutions_snapshots
mkdir -p num_non_dominated_snapshots
mkdir -p num_fronts_snapshots
mkdir -p populations_snapshots
mkdir -p num_elites_snapshots
mkdir -p num_mutants_snapshots
mkdir -p hypervolume
mkdir -p hypervolume_snapshots

for instance in ${instances[@]}
do
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command="./bin/exec/${solver}_solver_exec "
            command+="--instance instances/${instance}.txt "
            command+="--seed ${seed} "
            command+="--time-limit ${time_limit} "
            command+="--max-num-solutions ${max_num_solutions} "
            command+="--max-num-snapshots ${max_num_snapshots} "
            command+="--population-size ${population_size} "
            command+="--statistics statistics/${instance}_${solver}_${seed}.txt "
            command+="--solutions solutions/${instance}_${solver}_${seed}_ "
            command+="--pareto pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--num-non-dominated-snapshots num_non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--num-fronts-snapshots num_fronts_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--populations-snapshots populations_snapshots/${instance}_${solver}_${seed}.txt "
            if [ $solver = "nspso" ]
            then
                command+="--memory "
            fi
            if [ $solver = "moead" ]
            then
                command+="--preserve-diversity "
            fi
            if [ $solver = "mhaco" ]
            then
                command+="--memory "
            fi
            if [ $solver = "nsmpbrkga" ]
            then
                command+="--num-elites-snapshots num_elites_snapshots/${instance}_${solver}_${seed}.txt "
                command+="--num-mutants-snapshots num_mutants_snapshots/${instance}_${solver}_${seed}.txt "
            fi
            eval $command
        done
    done
    command="./bin/exec/hypervolume_calculator_exec "
    command+="--instance instances/${instance}.txt "
    i=0;
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command+="--pareto-${i} pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${i} best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--hypervolume-${i} hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${i} hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            i=$((i+1))
        done
    done
    eval $command
    for solver in ${solvers[@]}
    do
        command="./bin/exec/results_aggregator_exec "
        command+="--hypervolumes hypervolume/${instance}_${solver}.txt "
        command+="--hypervolume-statistics hypervolume/${instance}_${solver}_stats.txt "
        command+="--statistics-best statistics/${instance}_${solver}_best.txt "
        command+="--statistics-median statistics/${instance}_${solver}_median.txt "
        command+="--pareto-best pareto/${instance}_${solver}_best.txt "
        command+="--pareto-median pareto/${instance}_${solver}_median.txt "
        command+="--hypervolume-snapshots-best hypervolume_snapshots/${instance}_${solver}_best.txt "
        command+="--hypervolume-snapshots-median hypervolume_snapshots/${instance}_${solver}_median.txt "
        command+="--best-solutions-snapshots-best best_solutons_snapshots/${instance}_${solver}_best_ "
        command+="--best-solutions-snapshots-median best_solutions_snapshots/${instance}_${solver}_median_ "
        command+="--num-non-dominated-snapshots-best num_non_dominated_snapshots/${instance}_${solver}_best.txt "
        command+="--num-non-dominated-snapshots-median num_non_dominated_snapshots/${instance}_${solver}_median.txt "
        command+="--populations-snapshots-best populations/${instance}_${solver}_best_ "
        command+="--populations-snapshots-median populations/${instance}_${solver}_median_ "
        command+="--num-fronts-snapshots-best num_fronts_snapshots/${instance}_${solver}_best.txt "
        command+="--num-fronts-snapshots-median num_fronts_snapshots/${instance}_${solver}_median.txt "
        if [ $solver = "nsmpbrkga" ]
        then
            command+="--num-elites-snapshots-best num_elites_snapshots/${instance}_${solver}_best.txt "
            command+="--num-elites-snapshots-median num_elites_snapshots/${instance}_${solver}_median.txt "
            command+="--num-mutants-snapshots-best num_mutants_snapshots/${instance}_${solver}_best.txt "
            command+="--num-mutants-snapshots-median num_mutants_snapshots/${instance}_${solver}_median.txt "
        fi
        i=0;
        for seed in ${seeds[@]}
        do
            command+="--statistics-${i} statistics/${instance}_${solver}_${seed}.txt "
            command+="--pareto-${i} pareto/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-${i} hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${i} hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${i} best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--num-non-dominated-snapshots-${i} num_non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            if [ $solver = "nsmpbrkga" ]
            command+="--populations-snapshots-${i} populations_snapshots/${instance}_${solver}_${seed}_ "
            then
                command+="--num-elites-snapshots-${i} num_elites_snapshots/${instance}_${solver}_${seed}.txt "
                command+="--num-mutants-snapshots-${i} num_mutants_snapshots/${instance}_${solver}_${seed}.txt "
            fi
            i=$((i+1))
        done
        eval $command
    done
done

