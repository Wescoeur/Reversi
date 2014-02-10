# Release/Debug

DEBUG=yes

# Directories

SRC_DIR = SRC
INC_DIR = INC
OBJ_DIR = OBJ
BIN_DIR = BIN

# Compilation flags

CXX = gcc
CXXFLAGS = -Wall -Wextra -pedantic -ansi
LDFLAGS = 

ifeq ($(DEBUG), yes)
	CXXFLAGS += -g -DDEBUG
else
	CXXFLAGS += -DNDEBUG -O2 -s
endif

# Sources & Headers & Bin

SRC = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
OBJ = $(addsuffix .o, $(basename $(subst ${SRC_DIR}, ${OBJ_DIR}, ${SRC})))
BIN = tp1

# Make

.PHONY: clean mrproper
.SUFFIXES:

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)/
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o ${BIN_DIR}/${BIN} $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)/
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@rm -rf $(SRC_DIR)/*~ $(SRC_DIR)/*# $(INC_DIR)/*~ $(INC_DIR)/*# *~ *#

mrproper: clean
	@rm -rf $(BIN_DIR)/$(BIN)

rebuild: mrproper all
