#!/bin/bash

seed=666;
timeLimit=3600;

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p capturedParetos
mkdir -p hypervolume
mkdir -p capturedHypervolumes

./bin/exec/Christofides_Solver_Exec --instance instances/kroAC100.txt \
                                    --seed $seed --time-limit $timeLimit \
                                    --max-num-solutions 1000 --capture-enabled \
                                    --max-num-weights 1000 \
                                    --statistics statistics/christofidesA.txt \
                                    --solutions solutions/christofidesA \
                                    --pareto pareto/christofidesA.txt \
                                    --captured-paretos \
                                        capturedParetos/christofidesA;

./bin/exec/Christofides_Solver_Exec --instance instances/kroAC100.txt \
                                    --seed $seed --time-limit 360 \
                                    --max-num-solutions 100 --capture-enabled \
                                    --max-num-weights 100 \
                                    --statistics statistics/christofidesB.txt \
                                    --solutions solutions/christofidesB \
                                    --pareto pareto/christofidesB.txt \
                                    --captured-paretos \
                                        capturedParetos/christofidesB;

