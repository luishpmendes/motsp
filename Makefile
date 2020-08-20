CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
BRKGAINC=-I ../nsbrkga_mp_ipr/nsbrkga_mp_ipr
LEMONINC=-I /opt/lemon/include -L /opt/lemon/lib -lemon
GRBINC=-I /opt/gurobi911/linux64/include/ -L /opt/gurobi911/linux64/lib -lgurobi_c++ -lgurobi91 -lm
PAGMOINC=-pthread -lpagmo -lboost_serialization -ltbb -I /opt/pagmo/include -I /opt/boost/include -L /opt/pagmo/lib -L /opt/boost/lib -Wl,-R/opt/pagmo/lib
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
	$(CPP) $(CARGS) -c $< -o $@ $(BRKGAINC) $(LEMONINC) $(GRBINC) $(PAGMOINC)
	@echo

$(BIN)/test/Instance_Test : $(BIN)/instance/Instance.o \
                            $(BIN)/test/Instance_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/Instance_Test
	@echo

Instance_Test : clean $(BIN)/test/Instance_Test

$(BIN)/test/Solution_Test : $(BIN)/instance/Instance.o \
                            $(BIN)/solution/Solution.o \
                            $(BIN)/test/Solution_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/Solution_Test
	@echo

Solution_Test : clean $(BIN)/test/Solution_Test

$(BIN)/test/TwoOpt_Test : $(BIN)/instance/Instance.o \
                          $(BIN)/solution/Solution.o \
                          $(BIN)/solver/local_search/TwoOpt.o \
                          $(BIN)/test/TwoOpt_Test.o
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/TwoOpt_Test
	@echo

TwoOpt_Test : clean $(BIN)/test/TwoOpt_Test

$(BIN)/test/Christofides_Solver_Test : $(BIN)/instance/Instance.o \
                                       $(BIN)/solution/Solution.o \
                                       $(BIN)/solver/local_search/TwoOpt.o \
                                       $(BIN)/solver/Solver.o \
                                       $(BIN)/solver/weighted_sum/christofides/Christofides_Solver.o \
                                       $(BIN)/test/Christofides_Solver_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/Christofides_Solver_Test
	@echo

Christofides_Solver_Test : clean $(BIN)/test/Christofides_Solver_Test

$(BIN)/exec/Christofides_Solver_Exec : $(BIN)/instance/Instance.o \
                                       $(BIN)/solution/Solution.o \
                                       $(BIN)/solver/local_search/TwoOpt.o \
                                       $(BIN)/solver/Solver.o \
                                       $(BIN)/solver/weighted_sum/christofides/Christofides_Solver.o \
                                       $(BIN)/utils/ArgumentParser.o \
                                       $(BIN)/exec/Christofides_Solver_Exec.o 
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS)
	@echo

Christofides_Solver_Exec : clean $(BIN)/exec/Christofides_Solver_Exec

$(BIN)/test/BnC_Solver_Test : $(BIN)/instance/Instance.o \
                              $(BIN)/solution/Solution.o \
                              $(BIN)/solver/local_search/TwoOpt.o \
                              $(BIN)/solver/Solver.o \
                              $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Callback.o \
                              $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Solver.o \
                              $(BIN)/test/BnC_Solver_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS) $(GRBINC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/BnC_Solver_Test
	@echo

BnC_Solver_Test : clean $(BIN)/test/BnC_Solver_Test

$(BIN)/exec/BnC_Solver_Exec : $(BIN)/instance/Instance.o \
                              $(BIN)/solution/Solution.o \
                              $(BIN)/solver/local_search/TwoOpt.o \
                              $(BIN)/solver/Solver.o \
                              $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Callback.o \
                              $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Solver.o \
                              $(BIN)/utils/ArgumentParser.o \
                              $(BIN)/exec/BnC_Solver_Exec.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS) $(GRBINC)
	@echo

BnC_Solver_Exec : clean $(BIN)/exec/BnC_Solver_Exec

$(BIN)/test/NSGA2_Solver_Test : $(BIN)/instance/Instance.o \
                                $(BIN)/solution/Solution.o \
                                $(BIN)/solver/local_search/TwoOpt.o \
                                $(BIN)/solver/Solver.o \
                                $(BIN)/solver/weighted_sum/christofides/Christofides_Solver.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Callback.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Solver.o \
                                $(BIN)/solver/nsga2/Problem.o \
                                $(BIN)/solver/nsga2/NSGA2_Solver.o \
                                $(BIN)/test/NSGA2_Solver_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS) $(PAGMOINC) $(GRBINC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/NSGA2_Solver_Test
	@echo

NSGA2_Solver_Test : clean $(BIN)/test/NSGA2_Solver_Test

$(BIN)/exec/NSGA2_Solver_Exec : $(BIN)/instance/Instance.o \
                                $(BIN)/solution/Solution.o \
                                $(BIN)/solver/local_search/TwoOpt.o \
                                $(BIN)/solver/Solver.o \
                                $(BIN)/solver/weighted_sum/christofides/Christofides_Solver.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Callback.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Solver.o \
                                $(BIN)/solver/nsga2/Problem.o \
                                $(BIN)/solver/nsga2/NSGA2_Solver.o \
                                $(BIN)/utils/ArgumentParser.o \
                                $(BIN)/exec/NSGA2_Solver_Exec.o 
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(PAGMOINC) $(GRBINC)
	@echo

NSGA2_Solver_Exec : clean $(BIN)/exec/NSGA2_Solver_Exec

$(BIN)/test/NSPSO_Solver_Test : $(BIN)/instance/Instance.o \
                                $(BIN)/solution/Solution.o \
                                $(BIN)/solver/local_search/TwoOpt.o \
                                $(BIN)/solver/Solver.o \
                                $(BIN)/solver/weighted_sum/christofides/Christofides_Solver.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Callback.o \
                                $(BIN)/solver/weighted_sum/branch-and-cut/BnC_Solver.o \
                                $(BIN)/solver/nspso/Problem.o \
                                $(BIN)/solver/nspso/NSPSO_Solver.o \
                                $(BIN)/test/NSPSO_Solver_Test.o 
	@echo "--> Linking objects..." 
	$(CPP) -o $@ $^ $(CARGS) $(PAGMOINC) $(GRBINC)
	@echo
	@echo "--> Running test..."
	$(BIN)/test/NSPSO_Solver_Test
	@echo

NSPSO_Solver_Test : clean $(BIN)/test/NSPSO_Solver_Test

tests : Instance_Test \
        Solution_Test \
        TwoOpt_Test \
        Christofides_Solver_Test \
        BnC_Solver_Test \
        NSGA2_Solver_Test \
        NSPSO_Solver_Test

execs : Christofides_Solver_Exec \
        BnC_Solver_Exec \
        NSGA2_Solver_Exec

all : tests execs

