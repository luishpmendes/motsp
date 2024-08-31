#!/bin/bash

instances=(kroAB100 kroAB200 kroAB500 kroAB1000 kroAB2000 kroABC100 kroABC200 kroABC500 kroABC1000 kroABC2000 kroABCD100 kroABCD200 kroABCD500 kroABCD1000 kroABCD2000)
solvers=(nsga2 nspso moead mhaco ihs nsbrkga)
seeds=(305089489 511812191 608055156 467424509 944441939 414977408 819312498 562386085 287613914 755772793)
versions=(best median)

num_processes=6

time_limit=900
max_num_solutions=500
max_num_snapshots=30
max_ref_solutions=800

path=$(dirname $(realpath $0))

mkdir -p ${path}/statistics
# mkdir -p ${path}/solutions
mkdir -p ${path}/pareto
mkdir -p ${path}/best_solutions_snapshots
mkdir -p ${path}/num_non_dominated_snapshots
mkdir -p ${path}/num_fronts_snapshots
mkdir -p ${path}/populations_snapshots
mkdir -p ${path}/num_elites_snapshots
mkdir -p ${path}/hypervolume
mkdir -p ${path}/hypervolume_snapshots
mkdir -p ${path}/igd_plus
mkdir -p ${path}/igd_plus_snapshots
mkdir -p ${path}/multiplicative_epsilon
mkdir -p ${path}/multiplicative_epsilon_snapshots
mkdir -p ${path}/metrics
mkdir -p ${path}/metrics_snapshots

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do

            command="${path}/bin/exec/${solver}_solver_exec "
            command+="--instance ${path}/instances/${instance}.txt "
            command+="--seed ${seed} "
            command+="--time-limit ${time_limit} "
            command+="--max-num-solutions ${max_num_solutions} "
            command+="--max-num-snapshots ${max_num_snapshots} "
            command+="--statistics ${path}/statistics/${instance}_${solver}_${seed}.txt "
            # command+="--solutions ${path}/solutions/${instance}_${solver}_${seed}_ "
            command+="--pareto ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--num-non-dominated-snapshots ${path}/num_non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--num-fronts-snapshots ${path}/num_fronts_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--populations-snapshots ${path}/populations_snapshots/${instance}_${solver}_${seed}_ "
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
            if [ $solver = "nsbrkga" ]
            then
                command+="--num-elites-snapshots ${path}/num_elites_snapshots/${instance}_${solver}_${seed}.txt "
            fi
            if [ $i -lt $num_processes ]
            then
                commands[$i]+="$command"
            else
                commands[$((i%num_processes))]+=" && $command"
            fi
            i=$((i+1))
        done
    done
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do 
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

instances=(kroAB100 kroAB200 kroAB500 kroAB1000 kroAB2000 kroABC100 kroABC200 kroABC500 kroABC1000 kroABC2000 kroABCD100 kroABCD200 kroABCD500 kroABCD1000 kroABCD2000)
solvers=(nsga2 nspso moead mhaco ihs nsbrkga)

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    command="${path}/bin/exec/reference_pareto_front_calculator_exec "
    command+="--instance ${path}/instances/${instance}.txt "
    command+="--max-num-solutions ${max_ref_solutions} "
    j=0;
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command+="--pareto-${j} ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${j} ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--reference-pareto ${path}/pareto/${instance}.txt "
            j=$((j+1))
        done
    done
    if [ $i -lt $num_processes ]
    then
        commands[$i]+="$command"
    else
        commands[$((i%num_processes))]+=" && $command"
    fi
    i=$((i+1))
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    command="${path}/bin/exec/hypervolume_calculator_exec "
    command+="--instance ${path}/instances/${instance}.txt "
    command+="--reference-pareto ${path}/pareto/${instance}.txt "
    j=0;
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command+="--pareto-${j} ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${j} ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--hypervolume-${j} ${path}/hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${j} ${path}/hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            j=$((j+1))
        done
    done
    if [ $i -lt $num_processes ]
    then
        commands[$i]+="$command"
    else
        commands[$((i%num_processes))]+=" && $command"
    fi
    i=$((i+1))
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do 
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    command="${path}/bin/exec/modified_generational_distance_calculator_exec "
    command+="--instance ${path}/instances/${instance}.txt "
    command+="--reference-pareto ${path}/pareto/${instance}.txt "
    j=0;
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command+="--pareto-${j} ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${j} ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--igd-plus-${j} ${path}/igd_plus/${instance}_${solver}_${seed}.txt "
            command+="--igd-plus-snapshots-${j} ${path}/igd_plus_snapshots/${instance}_${solver}_${seed}.txt "
            j=$((j+1))
        done
    done
    if [ $i -lt $num_processes ]
    then
        commands[$i]+="$command"
    else
        commands[$((i%num_processes))]+=" && $command"
    fi
    i=$((i+1))
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    command="${path}/bin/exec/multiplicative_epsilon_calculator_exec "
    command+="--instance ${path}/instances/${instance}.txt "
    command+="--reference-pareto ${path}/pareto/${instance}.txt "
    j=0;
    for solver in ${solvers[@]}
    do
        for seed in ${seeds[@]}
        do
            command+="--pareto-${j} ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${j} ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--multiplicative-epsilon-${j} ${path}/multiplicative_epsilon/${instance}_${solver}_${seed}.txt "
            command+="--multiplicative-epsilon-snapshots-${j} ${path}/multiplicative_epsilon_snapshots/${instance}_${solver}_${seed}.txt "
            j=$((j+1))
        done
    done
    if [ $i -lt $num_processes ]
    then
        commands[$i]+="$command"
    else
        commands[$((i%num_processes))]+=" && $command"
    fi
    i=$((i+1))
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

commands=()

for ((i=0;i<num_processes;i++))
do
    commands[$i]="("
done

i=0

for instance in ${instances[@]}
do
    for solver in ${solvers[@]}
    do
        command="${path}/bin/exec/results_aggregator_exec "
        command+="--hypervolumes ${path}/hypervolume/${instance}_${solver}.txt "
        command+="--hypervolume-statistics ${path}/hypervolume/${instance}_${solver}_stats.txt "
        command+="--igd-pluses ${path}/igd_plus/${instance}_${solver}.txt "
        command+="--igd-pluses-statistics ${path}/igd_plus/${instance}_${solver}_stats.txt "
        command+="--multiplicative-epsilons ${path}/multiplicative_epsilon/${instance}_${solver}.txt "
        command+="--multiplicative-epsilons-statistics ${path}/multiplicative_epsilon/${instance}_${solver}_stats.txt "
        command+="--statistics-best ${path}/statistics/${instance}_${solver}_best.txt "
        command+="--statistics-median ${path}/statistics/${instance}_${solver}_median.txt "
        command+="--pareto-best ${path}/pareto/${instance}_${solver}_best.txt "
        command+="--pareto-median ${path}/pareto/${instance}_${solver}_median.txt "
        command+="--hypervolume-snapshots-best ${path}/hypervolume_snapshots/${instance}_${solver}_best.txt "
        command+="--hypervolume-snapshots-median ${path}/hypervolume_snapshots/${instance}_${solver}_median.txt "
        command+="--best-solutions-snapshots-best ${path}/best_solutions_snapshots/${instance}_${solver}_best_ "
        command+="--best-solutions-snapshots-median ${path}/best_solutions_snapshots/${instance}_${solver}_median_ "
        command+="--num-non-dominated-snapshots-best ${path}/num_non_dominated_snapshots/${instance}_${solver}_best.txt "
        command+="--num-non-dominated-snapshots-median ${path}/num_non_dominated_snapshots/${instance}_${solver}_median.txt "
        command+="--populations-snapshots-best ${path}/populations_snapshots/${instance}_${solver}_best_ "
        command+="--populations-snapshots-median ${path}/populations_snapshots/${instance}_${solver}_median_ "
        command+="--num-fronts-snapshots-best ${path}/num_fronts_snapshots/${instance}_${solver}_best.txt "
        command+="--num-fronts-snapshots-median ${path}/num_fronts_snapshots/${instance}_${solver}_median.txt "
        if [ $solver = "nsbrkga" ]
        then
            command+="--num-elites-snapshots-best ${path}/num_elites_snapshots/${instance}_${solver}_best.txt "
            command+="--num-elites-snapshots-median ${path}/num_elites_snapshots/${instance}_${solver}_median.txt "
        fi
        j=0;
        for seed in ${seeds[@]}
        do
            command+="--statistics-${j} ${path}/statistics/${instance}_${solver}_${seed}.txt "
            command+="--pareto-${j} ${path}/pareto/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-${j} ${path}/hypervolume/${instance}_${solver}_${seed}.txt "
            command+="--hypervolume-snapshots-${j} ${path}/hypervolume_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--igd-plus-${j} ${path}/igd_plus/${instance}_${solver}_${seed}.txt "
            command+="--igd-plus-snapshots-${j} ${path}/igd_plus_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--multiplicative-epsilon-${j} ${path}/multiplicative_epsilon/${instance}_${solver}_${seed}.txt "
            command+="--multiplicative-epsilon-snapshots-${j} ${path}/multiplicative_epsilon_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--best-solutions-snapshots-${j} ${path}/best_solutions_snapshots/${instance}_${solver}_${seed}_ "
            command+="--num-non-dominated-snapshots-${j} ${path}/num_non_dominated_snapshots/${instance}_${solver}_${seed}.txt "
            command+="--populations-snapshots-${j} ${path}/populations_snapshots/${instance}_${solver}_${seed}_ "
            command+="--num-fronts-snapshots-${j} ${path}/num_fronts_snapshots/${instance}_${solver}_${seed}.txt "
            if [ $solver = "nsbrkga" ]
            then
                command+="--num-elites-snapshots-${j} ${path}/num_elites_snapshots/${instance}_${solver}_${seed}.txt "
            fi
            j=$((j+1))
        done
        if [ $i -lt $num_processes ]
        then
            commands[$i]+="$command"
        else
            commands[$((i%num_processes))]+=" && $command"
        fi
        i=$((i+1))
    done
done

for ((i=0;i<num_processes;i++))
do
    commands[$i]+=") &>> ${path}/log_${i}.txt"
done

final_command=""

for ((i=0;i<num_processes;i++))
do 
    command=${commands[$i]}
    final_command+="$command & "
done

eval $final_command

wait

python3 ${path}/plotter_hypervolume.py &
python3 ${path}/plotter_hypervolume_snapshots.py &
python3 ${path}/plotter_igd_plus.py &
python3 ${path}/plotter_igd_plus_snapshots.py &
python3 ${path}/plotter_multiplicative_epsilon.py &
python3 ${path}/plotter_multiplicative_epsilon_snapshots.py &
python3 ${path}/plotter_metrics.py &
python3 ${path}/plotter_metrics_snapshots.py &
python3 ${path}/plotter_num_non_dominated_snapshots.py &
python3 ${path}/plotter_num_fronts_snapshots.py &
python3 ${path}/plotter_num_elites_snapshots.py &
python3 ${path}/plotter_pareto.py &
python3 ${path}/plotter_best_solutions_snapshots.py &
python3 ${path}/plotter_populations_snapshots.py

wait

for instance in ${instances[@]}
do
    for version in ${versions[@]}
    do
        ffmpeg -y -r 5 -i ${path}/best_solutions_snapshots/${instance}_${version}_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/best_solutions_snapshots/${instance}_${version}.mp4 &
        ffmpeg -y -r 5 -i ${path}/populations_snapshots/${instance}_${version}_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/populations_snapshots/${instance}_${version}.mp4

        wait

        rm ${path}/best_solutions_snapshots/${instance}_${version}_*.png &
        rm ${path}/populations_snapshots/${instance}_${version}_*.png

        wait
    done
done

ffmpeg -y -r 5 -i ${path}/hypervolume_snapshots/snapshot_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/hypervolume_snapshots/hypervolume.mp4 &
ffmpeg -y -r 5 -i ${path}/igd_plus_snapshots/snapshot_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/igd_plus_snapshots/igd_plus.mp4 &
ffmpeg -y -r 5 -i ${path}/multiplicative_epsilon_snapshots/snapshot_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/multiplicative_epsilon_snapshots/multiplicative_epsilon.mp4 &
ffmpeg -y -r 5 -i ${path}/metrics_snapshots/raincloud_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/metrics_snapshots/raincloud.mp4 &
ffmpeg -y -r 5 -i ${path}/metrics_snapshots/scatter_%d.png -c:v libx264 -vf fps=60 -pix_fmt yuv420p ${path}/metrics_snapshots/scatter.mp4

wait

rm ${path}/hypervolume_snapshots/snapshot_*.png &
rm ${path}/igd_plus_snapshots/snapshot_*.png &
rm ${path}/multiplicative_epsilon_snapshots/snapshot_*.png &
rm ${path}/metrics_snapshots/raincloud_*.png &
rm ${path}/metrics_snapshots/scatter_*.png

wait

python3 ${path}/metrics_stats.py > ${path}/metrics_stats.txt
