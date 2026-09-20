# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
TARGET = cpu_scheduler.exe
TEST_TARGET = test_process.exe

# Source Files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/core/Process.cpp \
       $(SRC_DIR)/core/ProcessManager.cpp

# Object Files
OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/core/Process.o \
       $(BUILD_DIR)/core/ProcessManager.o

TEST_OBJS = $(BUILD_DIR)/tests/test_process.o \
            $(BUILD_DIR)/core/Process.o \
            $(BUILD_DIR)/core/ProcessManager.o

# Default Target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/core/Process.o: $(SRC_DIR)/core/Process.cpp
	@if not exist "$(BUILD_DIR)\core" mkdir "$(BUILD_DIR)\core"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/core/ProcessManager.o: $(SRC_DIR)/core/ProcessManager.cpp
	@if not exist "$(BUILD_DIR)\core" mkdir "$(BUILD_DIR)\core"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_process.o: $(TEST_DIR)/test_process.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del /f /q $(TARGET)
	@if exist $(TEST_TARGET) del /f /q $(TEST_TARGET)

.PHONY: all test clean
