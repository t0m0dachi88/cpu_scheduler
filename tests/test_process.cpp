#include <iostream>
#include <cassert>
#include <string>
#include "core/Process.h"
#include "core/ProcessManager.h"

// Simple lightweight assertion macro for educational DSA testing
#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL]: " << message << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

#define ASSERT_EQUAL(actual, expected, message) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL]: " << message << " (Expected: " << (expected) << ", Got: " << (actual) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

// Test Suite 1: Process Model Initialization & Metrics
bool testProcessDefaultConstructor() {
    Process p;
    ASSERT_EQUAL(p.id, 0, "Default PID should be 0");
    ASSERT_EQUAL(p.arrivalTime, 0, "Default arrival time should be 0");
    ASSERT_EQUAL(p.burstTime, 0, "Default burst time should be 0");
    ASSERT_EQUAL(p.priority, 1, "Default priority should be 1");
    ASSERT_EQUAL(p.remainingTime, 0, "Default remaining time should be 0");
    ASSERT_TRUE(p.state == ProcessState::NEW, "Default state should be NEW");
    return true;
}

bool testProcessParameterizedConstructorClamping() {
    // Normal input
    Process p1(1, 0, 5, 2);
    ASSERT_EQUAL(p1.id, 1, "PID should be 1");
    ASSERT_EQUAL(p1.arrivalTime, 0, "Arrival time should be 0");
    ASSERT_EQUAL(p1.burstTime, 5, "Burst time should be 5");
    ASSERT_EQUAL(p1.priority, 2, "Priority should be 2");
    ASSERT_EQUAL(p1.remainingTime, 5, "Remaining time should equal burst time");

    // Negative/Zero inputs should be clamped to valid lower bounds
    Process p2(-5, -3, 0, -1);
    ASSERT_EQUAL(p2.id, 1, "Negative PID should clamp to 1");
    ASSERT_EQUAL(p2.arrivalTime, 0, "Negative arrival time should clamp to 0");
    ASSERT_EQUAL(p2.burstTime, 1, "Zero/negative burst time should clamp to 1");
    ASSERT_EQUAL(p2.priority, 1, "Zero/negative priority should clamp to 1");
    return true;
}

bool testProcessMetricCalculation() {
    Process p(1, 2, 6, 1);
    p.completionTime = 10;
    p.calculateMetrics();

    // TAT = CT - AT = 10 - 2 = 8
    ASSERT_EQUAL(p.turnaroundTime, 8, "Turnaround time calculation incorrect");

    // WT = TAT - BT = 8 - 6 = 2
    ASSERT_EQUAL(p.waitingTime, 2, "Waiting time calculation incorrect");
    return true;
}

bool testProcessReset() {
    Process p(1, 0, 5, 2);
    p.remainingTime = 0;
    p.completionTime = 5;
    p.calculateMetrics();
    p.state = ProcessState::COMPLETED;

    p.reset();
    ASSERT_EQUAL(p.remainingTime, 5, "Reset remaining time should equal burst time");
    ASSERT_EQUAL(p.completionTime, 0, "Reset completion time should be 0");
    ASSERT_EQUAL(p.turnaroundTime, 0, "Reset turnaround time should be 0");
    ASSERT_EQUAL(p.waitingTime, 0, "Reset waiting time should be 0");
    ASSERT_TRUE(p.state == ProcessState::NEW, "Reset state should be NEW");
    return true;
}

bool testProcessStateToString() {
    ASSERT_EQUAL(processStateToString(ProcessState::NEW), "NEW", "State string NEW incorrect");
    ASSERT_EQUAL(processStateToString(ProcessState::READY), "READY", "State string READY incorrect");
    ASSERT_EQUAL(processStateToString(ProcessState::RUNNING), "RUNNING", "State string RUNNING incorrect");
    ASSERT_EQUAL(processStateToString(ProcessState::COMPLETED), "COMPLETED", "State string COMPLETED incorrect");
    return true;
}

// Test Suite 2: ProcessManager Dynamic Collection & Validation
bool testProcessManagerValidation() {
    ProcessManager pm;
    std::string err;

    // Duplicate PID
    pm.addProcess(1, 0, 5, 1);
    ASSERT_TRUE(!pm.validateProcessInput(1, 2, 4, 1, err), "Duplicate PID should be invalid");

    // Negative arrival
    ASSERT_TRUE(!pm.validateProcessInput(2, -1, 4, 1, err), "Negative arrival time should be invalid");

    // Zero burst
    ASSERT_TRUE(!pm.validateProcessInput(3, 0, 0, 1, err), "Zero burst time should be invalid");

    // Zero priority
    ASSERT_TRUE(!pm.validateProcessInput(4, 0, 5, 0, err), "Zero priority should be invalid");
    return true;
}

bool testProcessManagerDynamicResizing() {
    ProcessManager pm(2); // Small capacity = 2

    // Add 5 processes to force dynamic array resizing
    for (int i = 1; i <= 5; ++i) {
        ASSERT_TRUE(pm.addProcess(i, i - 1, 4, 1), "Failed to add process during dynamic resize");
    }

    ASSERT_EQUAL(pm.getCount(), 5, "Process count should be 5 after dynamic expansion");
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQUAL(pm.getProcessAt(i).id, i + 1, "Process ID mismatch after array resize");
    }
    return true;
}

bool testProcessManagerClearAndReset() {
    ProcessManager pm;
    pm.addProcess(1, 0, 5, 1);
    pm.addProcess(2, 1, 3, 2);

    pm.resetAll();
    ASSERT_EQUAL(pm.getProcessAt(0).state, ProcessState::NEW, "resetAll should set state to NEW");

    pm.clear();
    ASSERT_EQUAL(pm.getCount(), 0, "clear should reset count to 0");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     RUNNING PROCESS MODEL UNIT TESTS    \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total = 8;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Process Default Constructor", testProcessDefaultConstructor);
    runTest("Process Parameterized Constructor Clamping", testProcessParameterizedConstructorClamping);
    runTest("Process Metric Calculation (TAT & WT)", testProcessMetricCalculation);
    runTest("Process Reset Functionality", testProcessReset);
    runTest("Process State String Conversion", testProcessStateToString);
    runTest("ProcessManager Input Validation Rules", testProcessManagerValidation);
    runTest("ProcessManager Dynamic Resizing Memory", testProcessManagerDynamicResizing);
    runTest("ProcessManager Reset & Clear", testProcessManagerClearAndReset);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
