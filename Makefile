CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
INC=-I src
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

tests : instance_test \
        solution_test

all : tests

