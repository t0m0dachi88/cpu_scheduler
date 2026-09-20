# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
TARGET = cpu_scheduler.exe
TEST_PROCESS_TARGET = test_process.exe
TEST_QUEUE_TARGET = test_queue.exe
TEST_CIRCULAR_QUEUE_TARGET = test_circular_queue.exe
TEST_MIN_HEAP_TARGET = test_min_heap.exe

# Source Files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/core/Process.cpp \
       $(SRC_DIR)/core/ProcessManager.cpp

# Object Files
OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/core/Process.o \
       $(BUILD_DIR)/core/ProcessManager.o

TEST_PROCESS_OBJS = $(BUILD_DIR)/tests/test_process.o \
                     $(BUILD_DIR)/core/Process.o \
                     $(BUILD_DIR)/core/ProcessManager.o

TEST_QUEUE_OBJS = $(BUILD_DIR)/tests/test_queue.o \
                   $(BUILD_DIR)/core/Process.o

TEST_CIRCULAR_QUEUE_OBJS = $(BUILD_DIR)/tests/test_circular_queue.o \
                            $(BUILD_DIR)/core/Process.o

TEST_MIN_HEAP_OBJS = $(BUILD_DIR)/tests/test_min_heap.o \
                      $(BUILD_DIR)/core/Process.o

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

$(BUILD_DIR)/tests/test_queue.o: $(TEST_DIR)/test_queue.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_circular_queue.o: $(TEST_DIR)/test_circular_queue.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_min_heap.o: $(TEST_DIR)/test_min_heap.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_PROCESS_TARGET): $(TEST_PROCESS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_PROCESS_OBJS)

$(TEST_QUEUE_TARGET): $(TEST_QUEUE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_QUEUE_OBJS)

$(TEST_CIRCULAR_QUEUE_TARGET): $(TEST_CIRCULAR_QUEUE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_CIRCULAR_QUEUE_OBJS)

$(TEST_MIN_HEAP_TARGET): $(TEST_MIN_HEAP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_MIN_HEAP_OBJS)

test: $(TEST_PROCESS_TARGET) $(TEST_QUEUE_TARGET) $(TEST_CIRCULAR_QUEUE_TARGET) $(TEST_MIN_HEAP_TARGET)
	./$(TEST_PROCESS_TARGET)
	./$(TEST_QUEUE_TARGET)
	./$(TEST_CIRCULAR_QUEUE_TARGET)
	./$(TEST_MIN_HEAP_TARGET)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del /f /q $(TARGET)
	@if exist $(TEST_PROCESS_TARGET) del /f /q $(TEST_PROCESS_TARGET)
	@if exist $(TEST_QUEUE_TARGET) del /f /q $(TEST_QUEUE_TARGET)
	@if exist $(TEST_CIRCULAR_QUEUE_TARGET) del /f /q $(TEST_CIRCULAR_QUEUE_TARGET)
	@if exist $(TEST_MIN_HEAP_TARGET) del /f /q $(TEST_MIN_HEAP_TARGET)

.PHONY: all test clean
