# Set folder locations
INC_DIR := ./inc
SRC_DIR := ./src
OUT_DIR := ./out

# Set build parameters
CC := g++
CFLAGS := -I$(INC_DIR) -std=c++17

# Add INC_DIR prefix to all dependencies
DEPS = $(INC_DIR)/$(wildcard *.hpp)

# Add OUT_DIR prefix to all out files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)


# Convert SRC files to their corresponding out files
OUT_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(SRC_FILES))

# Build the executable
CpuLogger: $(OUT_FILES)
	@$(CC) -o $@ $^ $(CFLAGS)

# out files depend on source files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	$(CC) -c $(CFLAGS) -o $@ $< 

# Clean up, clean up, everybody clean up
.PHONY clean:
clean:
	@rm -rf $(OUT_DIR)/*.o