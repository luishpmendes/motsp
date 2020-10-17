#!/bin/bash

seed=666;
timeLimit=3600;

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p capturedParetos
mkdir -p hypervolume
mkdir -p capturedHypervolumes

./bin/exec/NSPSO_Solver_Exec --instance instances/vm1084AB.txt --seed $seed \
                             --time-limit $timeLimit \
                             --capture-enabled --population-size 1000 \
                             --initial-individuals-method 0 \
                             --initial-individuals-percentage 0.0 \
                             --initial-individuals-time-percentage 0.0 \
                             --statistics statistics/nspsoC.txt \
                             --solutions solutions/nspsoC \
                             --pareto pareto/nspsoC.txt \
                             --captured-paretos capturedParetos/nspsoC;

./bin/exec/NSPSO_Solver_Exec --instance instances/vm1084AB.txt --seed $seed \
                             --time-limit $timeLimit \
                             --capture-enabled --population-size 1000 \
                             --initial-individuals-method 1 \
                             --initial-individuals-percentage 0.1 \
                             --initial-individuals-time-percentage 0.1 \
                             --statistics statistics/nspsoD.txt \
                             --solutions solutions/nspsoD \
                             --pareto pareto/nspsoD.txt \
                             --captured-paretos capturedParetos/nspsoD;

