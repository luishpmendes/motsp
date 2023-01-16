#!/bin/bash

make instance_generator_exec

./bin/exec/instance_generator_exec --id C --n 150 --max-x 4000 --max-y 2000 --seed 999742196 > instances/kroC150.tsp

./bin/exec/instance_generator_exec --id D --n 150 --max-x 4000 --max-y 2000 --seed 702808410 > instances/kroD150.tsp

./bin/exec/instance_generator_exec --id C --n 200 --max-x 4000 --max-y 2000 --seed 954475932 > instances/kroC200.tsp

./bin/exec/instance_generator_exec --id D --n 200 --max-x 4000 --max-y 2000 --seed 378186061 > instances/kroD200.tsp

./bin/exec/instance_generator_exec --id C --n 300 --max-x 4000 --max-y 2000 --seed 410238801 > instances/kroC300.tsp

./bin/exec/instance_generator_exec --id D --n 300 --max-x 4000 --max-y 2000 --seed 946633883 > instances/kroD300.tsp

./bin/exec/instance_generator_exec --id C --n 400 --max-x 4000 --max-y 2000 --seed 735550519 > instances/kroC400.tsp

./bin/exec/instance_generator_exec --id D --n 400 --max-x 4000 --max-y 2000 --seed 574258310 > instances/kroD400.tsp

./bin/exec/instance_generator_exec --id C --n 500 --max-x 4000 --max-y 2000 --seed 153339159 > instances/kroC500.tsp

./bin/exec/instance_generator_exec --id D --n 500 --max-x 4000 --max-y 2000 --seed 128282148 > instances/kroD500.tsp

./bin/exec/instance_generator_exec --id C --n 750 --max-x 4000 --max-y 2000 --seed 691067723 > instances/kroC750.tsp

./bin/exec/instance_generator_exec --id D --n 750 --max-x 4000 --max-y 2000 --seed 217988585 > instances/kroD750.tsp

./bin/exec/instance_generator_exec --id C --n 1000 --max-x 4000 --max-y 2000 --seed 113599195 > instances/kroC1000.tsp

./bin/exec/instance_generator_exec --id D --n 1000 --max-x 4000 --max-y 2000 --seed 938939977 > instances/kroD1000.tsp

make clean
