#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "utils/Visualizer.h"
#include "schedulers/FCFS.h"
#include "schedulers/RoundRobin.h"
#include "schedulers/PriorityScheduler.h"
#include "core/ProcessManager.h"

#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL]: " << message \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

#define ASSERT_EQUAL(actual, expected, message) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL]: " << message \
                      << " (Expected: " << (expected) << ", Got: " << (actual) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

// TC-001: Verify Visualizer renders without crashing on FCFS execution history
bool testVisualizerFCFSRender() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);
    fcfs.runSimulation(pm);

    // Check execution history has steps recorded
    ASSERT_TRUE(!fcfs.getExecutionHistory().isEmpty(),
                "FCFS execution history should not be empty after simulation");

    // Verify step count: 3 processes, no idle → 3 ExecutionSteps
    ASSERT_EQUAL(fcfs.getExecutionHistory().size(), 3,
                 "FCFS should have 3 execution steps for 3 processes (no idle)");

    // Render to console — should not throw or crash
    Visualizer::renderGanttChart(fcfs, pm, "FCFS Test Chart");
    Visualizer::renderProcessTable(pm);

    return true;
}

// TC-002: Verify Visualizer handles CPU idle gap (gap between P1 finish and P2 arrival)
bool testVisualizerIdleGapHandling() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 1); // P1 runs t=0..3
    pm.addProcess(2, 6, 2, 1); // P2 arrives at t=6 (CPU idle t=3..6)
    fcfs.runSimulation(pm);

    // Execution history should have 3 steps: P1, IDLE, P2
    ASSERT_EQUAL(fcfs.getExecutionHistory().size(), 3,
                 "Should have 3 steps: P1 + IDLE + P2");

    ASSERT_EQUAL(fcfs.getIdleTime(), 3, "CPU idle time should be 3 units");
    ASSERT_EQUAL(fcfs.getTotalCpuTime(), 8, "Total CPU time should be 8 units");

    Visualizer::renderGanttChart(fcfs, pm, "FCFS Idle Gap Test");
    return true;
}

// TC-003: Verify collectToArray() returns correct forward-chronological step ordering
bool testVisualizerChronologicalOrdering() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 2, 1); // P1: t=0..2
    pm.addProcess(2, 2, 3, 1); // P2: t=2..5
    pm.addProcess(3, 5, 1, 1); // P3: t=5..6
    fcfs.runSimulation(pm);

    // Use Stack::collectToArray directly to verify LIFO ordering
    const Stack<ExecutionStep>& hist = fcfs.getExecutionHistory();
    ExecutionStep arr[10];
    int count = hist.collectToArray(arr, 10);

    ASSERT_EQUAL(count, 3, "Should collect 3 steps");

    // Stack is LIFO: top = most recently pushed = last executed = P3
    ASSERT_EQUAL(arr[0].pid, 3, "Top of stack (most recent) should be P3");
    ASSERT_EQUAL(arr[1].pid, 2, "Second should be P2");
    ASSERT_EQUAL(arr[2].pid, 1, "Bottom should be P1 (first pushed)");

    // After reversal (what renderGanttChart does internally) → chronological P1, P2, P3
    Visualizer::renderGanttChart(fcfs, pm, "Chronological Ordering Verification");
    return true;
}

// TC-004: Verify Visualizer renders Round Robin multi-quantum steps correctly
bool testVisualizerRoundRobinRender() {
    RoundRobin rr(2);
    ProcessManager pm;

    pm.addProcess(1, 0, 4, 1); // P1
    pm.addProcess(2, 0, 4, 1); // P2
    rr.runSimulation(pm);

    // RR with Q=2 on two 4-unit processes: P1(0-2), P2(2-4), P1(4-6), P2(6-8) = 4 slices
    ASSERT_EQUAL(rr.getExecutionHistory().size(), 4,
                 "Round Robin Q=2 on two 4-BT processes should produce 4 execution steps");

    Visualizer::renderGanttChart(rr, pm, "Round Robin Q=2 Test");
    return true;
}

// TC-005: Verify empty history is handled gracefully
bool testVisualizerEmptyHistory() {
    FCFS fcfs;
    ProcessManager pm;
    // Do NOT run simulation → history remains empty

    ASSERT_TRUE(fcfs.getExecutionHistory().isEmpty(),
                "Execution history should be empty before simulation");

    // renderGanttChart with empty history should print a message without crashing
    Visualizer::renderGanttChart(fcfs, pm, "Empty History Test");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "       RUNNING VISUALIZER UNIT TESTS    \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total  = 5;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ...\n";
        if (testFunc()) {
            std::cout << "[PASS]: " << name << "\n\n";
            passed++;
        } else {
            std::cout << "[FAIL]: " << name << "\n\n";
        }
    };

    runTest("Visualizer FCFS Render (No Crash)",        testVisualizerFCFSRender);
    runTest("Visualizer Idle Gap Handling",              testVisualizerIdleGapHandling);
    runTest("Visualizer Chronological Step Ordering",    testVisualizerChronologicalOrdering);
    runTest("Visualizer Round Robin Multi-Step Render",  testVisualizerRoundRobinRender);
    runTest("Visualizer Empty History Graceful Fallback",testVisualizerEmptyHistory);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
