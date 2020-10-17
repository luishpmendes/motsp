#!/bin/bash

seed=666;
timeLimit=3600;

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p capturedParetos
mkdir -p hypervolume
mkdir -p capturedHypervolumes

./bin/exec/NSGA2_Solver_Exec --instance instances/vm1084AB.txt --seed $seed \
                             --time-limit $timeLimit \
                             --capture-enabled --population-size 1000 \
                             --initial-individuals-method 0 \
                             --initial-individuals-percentage 0.0 \
                             --initial-individuals-time-percentage 0.0 \
                             --statistics statistics/nsgaC.txt \
                             --solutions solutions/nsgaC \
                             --pareto pareto/nsgaC.txt \
                             --captured-paretos capturedParetos/nsgaC;

./bin/exec/NSGA2_Solver_Exec --instance instances/vm1084AB.txt --seed $seed \
                             --time-limit $timeLimit \
                             --capture-enabled --population-size 1000 \
                             --initial-individuals-method 1 \
                             --initial-individuals-percentage 0.1 \
                             --initial-individuals-time-percentage 0.1 \
                             --statistics statistics/nsgaD.txt \
                             --solutions solutions/nsgaD \
                             --pareto pareto/nsgaD.txt \
                             --captured-paretos capturedParetos/nsgaD;

