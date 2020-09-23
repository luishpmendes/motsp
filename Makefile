CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
BRKGAINC=-I ../nsbrkga_mp_ipr/nsbrkga_mp_ipr
LEMONINC=-I /opt/lemon/include -L /opt/lemon/lib -lemon
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
	$(CPP) $(CARGS) -c $< -o $@ $(BRKGAINC) $(LEMONINC)
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

tests : Instance_Test \
        Solution_Test \
        TwoOpt_Test \
        Christofides_Solver_Test

all : tests

