CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
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
	$(CPP) $(CARGS) -c $< -o $@
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

tests : Instance_Test

all : tests

