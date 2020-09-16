# Set folder locations
OUT_DIR := ./out
INC_DIR := ./inc
SRC_DIR := ./src

# Set build parameters
CC := g++
CFLAGS := -I$(INC_DIR)

# Add INC_DIR prefix to all dependencies
_DEPS = frequency.hpp
DEPS = $(patsubst %, $(INC_DIR)/%, $(_DEPS))

# Add OUT_DIR prefix to all out files
_OBJ = main.o frequency.o
OBJ = $(patsubst %, $(OUT_DIR)/%, $(_OBJ))

# Build the executable
CpuLogger: $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)

# Build the .o files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

# Clean up, clean up, everybody clean up
.PHONY clean:
clean:
	@rm -rf $(OUT_DIR)/*