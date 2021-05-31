#!/bin/bash

solvers=(nspso)
instances=(kroAB200)
seeds=(2351389233)

time_limit=3600
population_size=1000
max_num_solutions=2500
max_num_snapshots=60

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p pareto_snapshots
mkdir -p non_dominated_snapshots
mkdir -p fronts_snapshots
mkdir -p elite_sizes_snapshots
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
            command+="--pareto-snapshots pareto_snapshots/${instance}_${solver}_${seed}_ "
            command+="--non-dominated-snapshots non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--fronts-snapshots fronts_snapshots/${instance}_${solver}_${seed}.txt "
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
                command+="--elite-sizes-snapshots elite_sizes_snapshots/${instance}_${solver}_${seed}.txt "
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
            command+="--pareto-snapshots-${i} pareto_snapshots/${instance}_${solver}_${seed}_ "
            command+="--hypervolume-${i} hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${i} hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            i=$((i+1))
        done
    done
#    eval $command
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
        command+="--non-dominated-snapshots-best non_dominated_snapshots/${instance}_${solver}_best.txt "
        command+="--non-dominated-snapshots-median non_dominated_snapshots/${instance}_${solver}_median.txt "
        command+="--fronts-snapshots-best fronts_snapshots/${instance}_${solver}_best.txt "
        command+="--fronts-snapshots-median fronts_snapshots/${instance}_${solver}_median.txt "
        if [ $solver = "nsmpbrkga" ]
        then
            command+="--elite-sizes-snapshots-best elite_sizes_snapshots/${instance}_${solver}_best.txt "
            command+="--elite-sizes-snapshots-median elite_sizes_snapshots/${instance}_${solver}_median.txt "
        fi
        i=0;
        for seed in ${seeds[@]}
        do
            command+="--statistics-${i} statistics/${instance}_${solver}_${seed}.txt "
            command+="--pareto-${i} pareto/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-${i} hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${i} hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--non-dominated-snapshots-${i} non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--fronts-snapshots-${i} fronts_snapshots/${instance}_${solver}_${seed}.txt "
            if [ $solver = "nsmpbrkga" ]
            then
                command+="--elite-sizes-snapshots-${i} elite_sizes_snapshots/${instance}_${solver}_${seed}.txt "
            fi
            i=$((i+1))
        done
#        eval $command
    done
done

