#!/bin/bash

make instance_parser_exec

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroB100.tsp \
    > instances/kroAB100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroC100.tsp \
    > instances/kroAC100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroD100.tsp \
    > instances/kroAD100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroE100.tsp \
    > instances/kroAE100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroB100.tsp \
    --instance-1 instances/kroC100.tsp \
    > instances/kroBC100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroB100.tsp \
    --instance-1 instances/kroD100.tsp \
    > instances/kroBD100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroB100.tsp \
    --instance-1 instances/kroE100.tsp \
    > instances/kroBE100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroC100.tsp \
    --instance-1 instances/kroD100.tsp \
    > instances/kroCD100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroC100.tsp \
    --instance-1 instances/kroE100.tsp \
    > instances/kroCE100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroD100.tsp \
    --instance-1 instances/kroE100.tsp \
    > instances/kroDE100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA150.tsp \
    --instance-1 instances/kroB150.tsp \
    > instances/kroAB150.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA200.tsp \
    --instance-1 instances/kroB200.tsp \
    > instances/kroAB200.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA300.tsp \
    --instance-1 instances/kroB300.tsp \
    > instances/kroAB300.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA400.tsp \
    --instance-1 instances/kroB400.tsp \
    > instances/kroAB400.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA500.tsp \
    --instance-1 instances/kroB500.tsp \
    > instances/kroAB500.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA750.tsp \
    --instance-1 instances/kroB750.tsp \
    > instances/kroAB750.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA1000.tsp \
    --instance-1 instances/kroB1000.tsp \
    > instances/kroAB1000.txt


./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroB100.tsp \
    --instance-2 instances/kroC100.tsp \
    > instances/kroABC100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA100.tsp \
    --instance-1 instances/kroB100.tsp \
    --instance-2 instances/kroC100.tsp \
    --instance-3 instances/kroD100.tsp \
    > instances/kroABCD100.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA150.tsp \
    --instance-1 instances/kroB150.tsp \
    --instance-2 instances/kroC150.tsp \
    > instances/kroABC150.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA150.tsp \
    --instance-1 instances/kroB150.tsp \
    --instance-2 instances/kroC150.tsp \
    --instance-3 instances/kroD150.tsp \
    > instances/kroABCD150.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA200.tsp \
    --instance-1 instances/kroB200.tsp \
    --instance-2 instances/kroC200.tsp \
    > instances/kroABC200.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA200.tsp \
    --instance-1 instances/kroB200.tsp \
    --instance-2 instances/kroC200.tsp \
    --instance-3 instances/kroD200.tsp \
    > instances/kroABCD200.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA300.tsp \
    --instance-1 instances/kroB300.tsp \
    --instance-2 instances/kroC300.tsp \
    > instances/kroABC300.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA300.tsp \
    --instance-1 instances/kroB300.tsp \
    --instance-2 instances/kroC300.tsp \
    --instance-3 instances/kroD300.tsp \
    > instances/kroABCD300.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA400.tsp \
    --instance-1 instances/kroB400.tsp \
    --instance-2 instances/kroC400.tsp \
    > instances/kroABC400.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA400.tsp \
    --instance-1 instances/kroB400.tsp \
    --instance-2 instances/kroC400.tsp \
    --instance-3 instances/kroD400.tsp \
    > instances/kroABCD400.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA500.tsp \
    --instance-1 instances/kroB500.tsp \
    --instance-2 instances/kroC500.tsp \
    > instances/kroABC500.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA500.tsp \
    --instance-1 instances/kroB500.tsp \
    --instance-2 instances/kroC500.tsp \
    --instance-3 instances/kroD500.tsp \
    > instances/kroABCD500.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA750.tsp \
    --instance-1 instances/kroB750.tsp \
    --instance-2 instances/kroC750.tsp \
    > instances/kroABC750.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA750.tsp \
    --instance-1 instances/kroB750.tsp \
    --instance-2 instances/kroC750.tsp \
    --instance-3 instances/kroD750.tsp \
    > instances/kroABCD750.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA1000.tsp \
    --instance-1 instances/kroB1000.tsp \
    --instance-2 instances/kroC1000.tsp \
    > instances/kroABC1000.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA1000.tsp \
    --instance-1 instances/kroB1000.tsp \
    --instance-2 instances/kroC1000.tsp \
    --instance-3 instances/kroD1000.tsp \
    > instances/kroABCD1000.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA2000.tsp \
    --instance-1 instances/kroB2000.tsp \
    > instances/kroAB2000.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA2000.tsp \
    --instance-1 instances/kroB2000.tsp \
    --instance-2 instances/kroC2000.tsp \
    > instances/kroABC2000.txt

./bin/exec/instance_parser_exec \
    --instance-0 instances/kroA2000.tsp \
    --instance-1 instances/kroB2000.tsp \
    --instance-2 instances/kroC2000.tsp \
    --instance-3 instances/kroD2000.tsp \
    > instances/kroABCD2000.txt

make clean
