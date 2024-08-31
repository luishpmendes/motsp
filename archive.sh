#!/bin/bash

id="31"

mkdir old/$id

mkdir old/$id/best_solutions_snapshots
cp best_solutions_snapshots/*.mp4 old/$id/best_solutions_snapshots

mkdir old/$id/hypervolume
cp hypervolume/*.png old/$id/hypervolume

mkdir old/$id/hypervolume_snapshots
cp hypervolume_snapshots/*.png old/$id/hypervolume_snapshots
cp hypervolume_snapshots/*.mp4 old/$id/hypervolume_snapshots

mkdir old/$id/igd_plus
cp igd_plus/*.png old/$id/igd_plus

mkdir old/$id/igd_plus_snapshots
cp igd_plus_snapshots/*.png old/$id/igd_plus_snapshots
cp igd_plus_snapshots/*.mp4 old/$id/igd_plus_snapshots

mkdir old/$id/multiplicative_epsilon
cp multiplicative_epsilon/*.png old/$id/multiplicative_epsilon

mkdir old/$id/multiplicative_epsilon_snapshots
cp multiplicative_epsilon_snapshots/*.png old/$id/multiplicative_epsilon_snapshots
cp multiplicative_epsilon_snapshots/*.mp4 old/$id/multiplicative_epsilon_snapshots

mkdir old/$id/metrics
cp metrics/* old/$id/metrics

mkdir old/$id/metrics_snapshots
cp metrics_snapshots/* old/$id/metrics_snapshots

mkdir old/$id/num_elites_snapshots
cp num_elites_snapshots/*.png old/$id/num_elites_snapshots

mkdir old/$id/num_fronts_snapshots
cp num_fronts_snapshots/*.png old/$id/num_fronts_snapshots

mkdir old/$id/num_non_dominated_snapshots
cp num_non_dominated_snapshots/*.png old/$id/num_non_dominated_snapshots

mkdir old/$id/pareto
cp pareto/*.png old/$id/pareto

mkdir old/$id/populations_snapshots
cp populations_snapshots/*.mp4 old/$id/populations_snapshots

mkdir old/$id/statistics
cp statistics/* old/$id/statistics

cp metrics_stats.txt old/$id


rm -rf best_solutions_snapshots
git rm -rf best_solutions_snapshots

rm -rf hypervolume
git rm -rf hypervolume

rm -rf hypervolume_snapshots
git rm -rf hypervolume_snapshots

rm -rf igd_plus
git rm -rf igd_plus

rm -rf igd_plus_snapshots
git rm -rf igd_plus_snapshots

rm -rf multiplicative_epsilon
git rm -rf multiplicative_epsilon

rm -rf multiplicative_epsilon_snapshots
git rm -rf multiplicative_epsilon_snapshots

rm -rf num_elites_snapshots
git rm -rf num_elites_snapshots

rm -rf num_fronts_snapshots
git rm -rf num_fronts_snapshots

rm -rf num_non_dominated_snapshots
git rm -rf num_non_dominated_snapshots

rm -rf pareto
git rm -rf pareto

rm -rf populations_snapshots
git rm -rf populations_snapshots

rm -rf solutions
git rm -rf solutions

git rm metrics_stats.txt

git rm log*.txt
