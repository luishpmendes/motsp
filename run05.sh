#!/bin/bash

seed=666;
timeLimit=3600;

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p capturedParetos
mkdir -p hypervolume
mkdir -p capturedHypervolumes

./bin/exec/NSBRKGA_MP_IPR_Solver_Exec --instance instances/kroAC100.txt \
                                      --seed $seed --time-limit $timeLimit \
                                      --capture-enabled \
                                      --population-size 1000 \
                                      --initial-individuals-method 0 \
                                      --initial-individuals-percentage 0.0 \
                                      --initial-individuals-time-percentage 0.0 \
                                      --elite-percentage 0.2 \
                                      --mutant-percentage 0.2 \
                                      --num-total-parents 2 \
                                      --num-elite-parents 1 \
                                      --num-populations 1 \
                                      --pr-percentage-pairs 0.0 \
                                      --pr-min-dist 0.0 --pr-percentage 0.0 \
                                      --pr-interval 0 --shake-interval 0 \
                                      --reset-interval 200 \
                                      --max-local-search-iterations 0 \
                                      --statistics statistics/nsbrkgaA.txt \
                                      --solutions solutions/nsbrkgaA \
                                      --pareto pareto/nsbrkgaA.txt \
                                      --captured-paretos capturedParetos/nsbrkgaA;

./bin/exec/NSBRKGA_MP_IPR_Solver_Exec --instance instances/kroAC100.txt \
                                      --seed $seed --time-limit $timeLimit \
                                      --capture-enabled \
                                      --population-size 1000 \
                                      --initial-individuals-method 1 \
                                      --initial-individuals-percentage 0.1 \
                                      --initial-individuals-time-percentage 0.1 \
                                      --elite-percentage 0.2 \
                                      --mutant-percentage 0.2 \
                                      --num-total-parents 2 \
                                      --num-elite-parents 1 \
                                      --num-populations 1 \
                                      --pr-percentage-pairs 0.5 \
                                      --pr-min-dist 0.0 --pr-percentage 0.0 \
                                      --pr-interval 0 --shake-interval 0 \
                                      --reset-interval 200 \
                                      --max-local-search-iterations 0 \
                                      --statistics statistics/nsbrkgaB.txt \
                                      --solutions solutions/nsbrkgaB \
                                      --pareto pareto/nsbrkgaB.txt \
                                      --captured-paretos capturedParetos/nsbrkgaB;

