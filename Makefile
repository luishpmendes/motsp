CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
BRKGAINC=-I ../nsmpbrkga/nsmpbrkga
LEMONINC=-I /opt/lemon/include -L /opt/lemon/lib -lemon
GRBINC=-I /opt/gurobi911/linux64/include/ -L /opt/gurobi911/linux64/lib -lgurobi_c++ -lgurobi91 -lm
INC=-I src $(BRKGAINC) $(LEMONINC) $(GRBINC)
MKDIR=mkdir -p
RM=rm -rf
SRC=$(PWD)/src
BIN=$(PWD)/bin

clean:
	@echo "--> Cleaning compiled..."
	$(RM) $(BIN)
	@echo

$(BIN)/%.o: $(SRC)/%.cpp
	@echo "--> Compiling $<..."
	$(MKDIR) $(@D)
	$(CPP) $(CARGS) -c $< -o $@ $(INC)
	@echo

$(BIN)/test/instance_test : $(BIN)/instance/instance.o \
                            $(BIN)/test/instance_test.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/instance_test
	@echo

instance_test : $(BIN)/test/instance_test

$(BIN)/test/solution_test : $(BIN)/instance/instance.o \
                            $(BIN)/solution/solution.o \
                            $(BIN)/test/solution_test.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/solution_test
	@echo

solution_test : $(BIN)/test/solution_test

$(BIN)/test/two_opt_test : $(BIN)/instance/instance.o \
                           $(BIN)/solution/solution.o \
                           $(BIN)/solver/local_search/two_opt.o \
                           $(BIN)/test/two_opt_test.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/two_opt_test
	@echo

two_opt_test : $(BIN)/test/two_opt_test

$(BIN)/test/christofides_solver_test : $(BIN)/instance/instance.o \
                                       $(BIN)/solution/solution.o \
                                       $(BIN)/solver/local_search/two_opt.o \
                                       $(BIN)/solver/solver.o \
                                       $(BIN)/solver/weighted_sum/christofides/christofides_solver.o \
                                       $(BIN)/test/christofides_solver_test.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/christofides_solver_test
	@echo

christofides_solver_test : $(BIN)/test/christofides_solver_test

$(BIN)/exec/christofides_solver_exec : $(BIN)/instance/instance.o \
                                       $(BIN)/solution/solution.o \
                                       $(BIN)/solver/local_search/two_opt.o \
                                       $(BIN)/solver/solver.o \
                                       $(BIN)/solver/weighted_sum/christofides/christofides_solver.o \
                                       $(BIN)/utils/argument_parser.o \
                                       $(BIN)/exec/christofides_solver_exec.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo

christofides_solver_exec : $(BIN)/exec/christofides_solver_exec

$(BIN)/test/branch_and_cut_solver_test : $(BIN)/instance/instance.o \
                                         $(BIN)/solution/solution.o \
                                         $(BIN)/solver/local_search/two_opt.o \
                                         $(BIN)/solver/solver.o \
                                         $(BIN)/solver/weighted_sum/branch_and_cut/branch_and_cut_callback.o \
                                         $(BIN)/solver/weighted_sum/branch_and_cut/branch_and_cut_solver.o \
                                         $(BIN)/test/branch_and_cut_solver_test.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/branch_and_cut_solver_test
	@echo

branch_and_cut_solver_test : $(BIN)/test/branch_and_cut_solver_test

tests : instance_test \
        solution_test \
        two_opt_test \
        christofides_solver_test \
        branch_and_cut_solver_test

execs : christofides_solver_exec

all : tests execs

