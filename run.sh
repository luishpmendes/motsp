#!/bin/bash

mkdir -p statistics
mkdir -p solutions
mkdir -p pareto
mkdir -p capturedParetos
mkdir -p hypervolume
mkdir -p capturedHypervolumes

./bin/exec/Hypervolume_Calculator_Exec --instance instances/kroAC100.txt \
                                       --pareto-0 pareto/christofidesA.txt \
                                       --pareto-1 pareto/bncA.txt \
                                       --pareto-2 pareto/nsgaA.txt \
                                       --pareto-3 pareto/nspsoA.txt \
                                       --pareto-4 pareto/nsbrkgaA.txt \
                                       --pareto-5 pareto/christofidesB.txt \
                                       --pareto-6 pareto/bncB.txt \
                                       --pareto-7 pareto/nsgaB.txt \
                                       --pareto-8 pareto/nspsoB.txt \
                                       --pareto-9 pareto/nsbrkgaB.txt \
                                       --captured-paretos-0 capturedParetos/christofidesA \
                                       --captured-paretos-1 capturedParetos/bncA \
                                       --captured-paretos-2 capturedParetos/nsgaA \
                                       --captured-paretos-3 capturedParetos/nspsoA \
                                       --captured-paretos-4 capturedParetos/nsbrkgaA \
                                       --captured-paretos-5 capturedParetos/christofidesB \
                                       --captured-paretos-6 capturedParetos/bncB \
                                       --captured-paretos-7 capturedParetos/nsgaB \
                                       --captured-paretos-8 capturedParetos/nspsoB \
                                       --captured-paretos-9 capturedParetos/nsbrkgaB \
                                       --hypervolume-0 hypervolume/christofidesA.txt \
                                       --hypervolume-1 hypervolume/bncA.txt \
                                       --hypervolume-2 hypervolume/nsgaA.txt \
                                       --hypervolume-3 hypervolume/nspsoA.txt \
                                       --hypervolume-4 hypervolume/nsbrkgaA.txt \
                                       --hypervolume-5 hypervolume/christofidesB.txt \
                                       --hypervolume-6 hypervolume/bncB.txt \
                                       --hypervolume-7 hypervolume/nsgaB.txt \
                                       --hypervolume-8 hypervolume/nspsoB.txt \
                                       --hypervolume-9 hypervolume/nsbrkgaB.txt \
                                       --captured-hypervolumes-0 capturedHypervolumes/christofidesA.txt \
                                       --captured-hypervolumes-1 capturedHypervolumes/bncA.txt \
                                       --captured-hypervolumes-2 capturedHypervolumes/nsgaA.txt \
                                       --captured-hypervolumes-3 capturedHypervolumes/nspsoA.txt \
                                       --captured-hypervolumes-4 capturedHypervolumes/nsbrkgaA.txt \
                                       --captured-hypervolumes-5 capturedHypervolumes/christofidesB.txt \
                                       --captured-hypervolumes-6 capturedHypervolumes/bncB.txt \
                                       --captured-hypervolumes-7 capturedHypervolumes/nsgaB.txt \
                                       --captured-hypervolumes-8 capturedHypervolumes/nspsoB.txt \
                                       --captured-hypervolumes-9 capturedHypervolumes/nsbrkgaB.txt;

./bin/exec/Hypervolume_Calculator_Exec --instance instances/kroAC100.txt \
                                       --pareto-0 pareto/christofidesC.txt \
                                       --pareto-1 pareto/bncC.txt \
                                       --pareto-2 pareto/nsgaC.txt \
                                       --pareto-3 pareto/nspsoC.txt \
                                       --pareto-4 pareto/nsbrkgaC.txt \
                                       --pareto-5 pareto/christofidesD.txt \
                                       --pareto-6 pareto/bncD.txt \
                                       --pareto-7 pareto/nsgaD.txt \
                                       --pareto-8 pareto/nspsoD.txt \
                                       --pareto-9 pareto/nsbrkgaD.txt \
                                       --captured-paretos-0 capturedParetos/christofidesC \
                                       --captured-paretos-1 capturedParetos/bncC \
                                       --captured-paretos-2 capturedParetos/nsgaC \
                                       --captured-paretos-3 capturedParetos/nspsoC \
                                       --captured-paretos-4 capturedParetos/nsbrkgaC \
                                       --captured-paretos-5 capturedParetos/christofidesD \
                                       --captured-paretos-6 capturedParetos/bncD \
                                       --captured-paretos-7 capturedParetos/nsgaD \
                                       --captured-paretos-8 capturedParetos/nspsoD \
                                       --captured-paretos-9 capturedParetos/nsbrkgaD \
                                       --hypervolume-0 hypervolume/christofidesC.txt \
                                       --hypervolume-1 hypervolume/bncC.txt \
                                       --hypervolume-2 hypervolume/nsgaC.txt \
                                       --hypervolume-3 hypervolume/nspsoC.txt \
                                       --hypervolume-4 hypervolume/nsbrkgaC.txt \
                                       --hypervolume-5 hypervolume/christofidesD.txt \
                                       --hypervolume-6 hypervolume/bncD.txt \
                                       --hypervolume-7 hypervolume/nsgaD.txt \
                                       --hypervolume-8 hypervolume/nspsoD.txt \
                                       --hypervolume-9 hypervolume/nsbrkgaD.txt \
                                       --captured-hypervolumes-0 capturedHypervolumes/christofidesC.txt \
                                       --captured-hypervolumes-1 capturedHypervolumes/bncC.txt \
                                       --captured-hypervolumes-2 capturedHypervolumes/nsgaC.txt \
                                       --captured-hypervolumes-3 capturedHypervolumes/nspsoC.txt \
                                       --captured-hypervolumes-4 capturedHypervolumes/nsbrkgaC.txt \
                                       --captured-hypervolumes-5 capturedHypervolumes/christofidesD.txt \
                                       --captured-hypervolumes-6 capturedHypervolumes/bncD.txt \
                                       --captured-hypervolumes-7 capturedHypervolumes/nsgaD.txt \
                                       --captured-hypervolumes-8 capturedHypervolumes/nspsoD.txt \
                                       --captured-hypervolumes-9 capturedHypervolumes/nsbrkgaD.txt;

