# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
TEST_BIN_DIR = $(BUILD_DIR)/tests_bin
TARGET = cpu_scheduler.exe

TEST_PROCESS_TARGET = $(TEST_BIN_DIR)/test_process.exe
TEST_QUEUE_TARGET = $(TEST_BIN_DIR)/test_queue.exe
TEST_CIRCULAR_QUEUE_TARGET = $(TEST_BIN_DIR)/test_circular_queue.exe
TEST_MIN_HEAP_TARGET = $(TEST_BIN_DIR)/test_min_heap.exe
TEST_STACK_TARGET = $(TEST_BIN_DIR)/test_stack.exe
TEST_SEARCH_SORT_TARGET = $(TEST_BIN_DIR)/test_search_sort.exe
TEST_SCHEDULER_BASE_TARGET = $(TEST_BIN_DIR)/test_scheduler_base.exe
TEST_FCFS_TARGET = $(TEST_BIN_DIR)/test_fcfs.exe
TEST_ROUND_ROBIN_TARGET = $(TEST_BIN_DIR)/test_round_robin.exe

# Source Files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/core/Process.cpp \
       $(SRC_DIR)/core/ProcessManager.cpp \
       $(SRC_DIR)/utils/SearchSort.cpp \
       $(SRC_DIR)/schedulers/FCFS.cpp \
       $(SRC_DIR)/schedulers/RoundRobin.cpp

# Object Files
OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/core/Process.o \
       $(BUILD_DIR)/core/ProcessManager.o \
       $(BUILD_DIR)/utils/SearchSort.o \
       $(BUILD_DIR)/schedulers/FCFS.o \
       $(BUILD_DIR)/schedulers/RoundRobin.o

TEST_PROCESS_OBJS = $(BUILD_DIR)/tests/test_process.o \
                     $(BUILD_DIR)/core/Process.o \
                     $(BUILD_DIR)/core/ProcessManager.o

TEST_QUEUE_OBJS = $(BUILD_DIR)/tests/test_queue.o \
                   $(BUILD_DIR)/core/Process.o

TEST_CIRCULAR_QUEUE_OBJS = $(BUILD_DIR)/tests/test_circular_queue.o \
                            $(BUILD_DIR)/core/Process.o

TEST_MIN_HEAP_OBJS = $(BUILD_DIR)/tests/test_min_heap.o \
                      $(BUILD_DIR)/core/Process.o

TEST_STACK_OBJS = $(BUILD_DIR)/tests/test_stack.o \
                   $(BUILD_DIR)/core/Process.o

TEST_SEARCH_SORT_OBJS = $(BUILD_DIR)/tests/test_search_sort.o \
                         $(BUILD_DIR)/core/Process.o \
                         $(BUILD_DIR)/utils/SearchSort.o

TEST_SCHEDULER_BASE_OBJS = $(BUILD_DIR)/tests/test_scheduler_base.o \
                            $(BUILD_DIR)/core/Process.o \
                            $(BUILD_DIR)/core/ProcessManager.o \
                            $(BUILD_DIR)/utils/SearchSort.o

TEST_FCFS_OBJS = $(BUILD_DIR)/tests/test_fcfs.o \
                  $(BUILD_DIR)/core/Process.o \
                  $(BUILD_DIR)/core/ProcessManager.o \
                  $(BUILD_DIR)/utils/SearchSort.o \
                  $(BUILD_DIR)/schedulers/FCFS.o

TEST_ROUND_ROBIN_OBJS = $(BUILD_DIR)/tests/test_round_robin.o \
                        $(BUILD_DIR)/core/Process.o \
                        $(BUILD_DIR)/core/ProcessManager.o \
                        $(BUILD_DIR)/utils/SearchSort.o \
                        $(BUILD_DIR)/schedulers/RoundRobin.o

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

$(BUILD_DIR)/utils/SearchSort.o: $(SRC_DIR)/utils/SearchSort.cpp
	@if not exist "$(BUILD_DIR)\utils" mkdir "$(BUILD_DIR)\utils"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/schedulers/FCFS.o: $(SRC_DIR)/schedulers/FCFS.cpp
	@if not exist "$(BUILD_DIR)\schedulers" mkdir "$(BUILD_DIR)\schedulers"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/schedulers/RoundRobin.o: $(SRC_DIR)/schedulers/RoundRobin.cpp
	@if not exist "$(BUILD_DIR)\schedulers" mkdir "$(BUILD_DIR)\schedulers"
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

$(BUILD_DIR)/tests/test_stack.o: $(TEST_DIR)/test_stack.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_search_sort.o: $(TEST_DIR)/test_search_sort.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_scheduler_base.o: $(TEST_DIR)/test_scheduler_base.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_fcfs.o: $(TEST_DIR)/test_fcfs.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/test_round_robin.o: $(TEST_DIR)/test_round_robin.cpp
	@if not exist "$(BUILD_DIR)\tests" mkdir "$(BUILD_DIR)\tests"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_PROCESS_TARGET): $(TEST_PROCESS_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_PROCESS_OBJS)

$(TEST_QUEUE_TARGET): $(TEST_QUEUE_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_QUEUE_OBJS)

$(TEST_CIRCULAR_QUEUE_TARGET): $(TEST_CIRCULAR_QUEUE_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_CIRCULAR_QUEUE_OBJS)

$(TEST_MIN_HEAP_TARGET): $(TEST_MIN_HEAP_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_MIN_HEAP_OBJS)

$(TEST_STACK_TARGET): $(TEST_STACK_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_STACK_OBJS)

$(TEST_SEARCH_SORT_TARGET): $(TEST_SEARCH_SORT_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SEARCH_SORT_OBJS)

$(TEST_SCHEDULER_BASE_TARGET): $(TEST_SCHEDULER_BASE_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SCHEDULER_BASE_OBJS)

$(TEST_FCFS_TARGET): $(TEST_FCFS_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_FCFS_OBJS)

$(TEST_ROUND_ROBIN_TARGET): $(TEST_ROUND_ROBIN_OBJS)
	@if not exist "$(BUILD_DIR)\tests_bin" mkdir "$(BUILD_DIR)\tests_bin"
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_ROUND_ROBIN_OBJS)

test: $(TEST_PROCESS_TARGET) $(TEST_QUEUE_TARGET) $(TEST_CIRCULAR_QUEUE_TARGET) $(TEST_MIN_HEAP_TARGET) $(TEST_STACK_TARGET) $(TEST_SEARCH_SORT_TARGET) $(TEST_SCHEDULER_BASE_TARGET) $(TEST_FCFS_TARGET) $(TEST_ROUND_ROBIN_TARGET)
	./$(TEST_PROCESS_TARGET)
	./$(TEST_QUEUE_TARGET)
	./$(TEST_CIRCULAR_QUEUE_TARGET)
	./$(TEST_MIN_HEAP_TARGET)
	./$(TEST_STACK_TARGET)
	./$(TEST_SEARCH_SORT_TARGET)
	./$(TEST_SCHEDULER_BASE_TARGET)
	./$(TEST_FCFS_TARGET)
	./$(TEST_ROUND_ROBIN_TARGET)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET) del /f /q $(TARGET)

.PHONY: all test clean
