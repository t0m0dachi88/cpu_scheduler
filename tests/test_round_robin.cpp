#include <iostream>
#include <cassert>
#include <cmath>
#include "schedulers/RoundRobin.h"
#include "core/ProcessManager.h"

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

#define ASSERT_NEAR(actual, expected, tolerance, message) \
    do { \
        if (std::abs((actual) - (expected)) > (tolerance)) { \
            std::cerr << "[FAIL]: " << message << " (Expected: " << (expected) << ", Got: " << (actual) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

bool testRRSection24BenchmarkWorkload() {
    RoundRobin rr(2); // Quantum = 2
    ProcessManager pm;

    // Benchmark workload from Prompt Section 24
    pm.addProcess(1, 0, 5, 2); // P1: AT=0, BT=5
    pm.addProcess(2, 1, 3, 1); // P2: AT=1, BT=3
    pm.addProcess(3, 2, 8, 3); // P3: AT=2, BT=8

    rr.runSimulation(pm);

    ASSERT_EQUAL(rr.getTotalCpuTime(), 16, "Total CPU time should be 16");
    ASSERT_EQUAL(rr.getIdleTime(), 0, "Idle time should be 0");

    // RR Execution Timeline:
    // t=0..2 (P1), t=2..4 (P2), t=4..6 (P3), t=6..8 (P1), t=8..9 (P2 finishes),
    // t=9..11 (P3), t=11..12 (P1 finishes), t=12..16 (P3 finishes)

    // P1: CT=12, TAT=12, WT=7
    Process p1 = pm.getProcessAt(0);
    ASSERT_EQUAL(p1.id, 1, "P1 should be at index 0");
    ASSERT_EQUAL(p1.completionTime, 12, "P1 CT should be 12");
    ASSERT_EQUAL(p1.turnaroundTime, 12, "P1 TAT should be 12");
    ASSERT_EQUAL(p1.waitingTime, 7, "P1 WT should be 7");

    // P2: CT=9, TAT=8, WT=5
    Process p2 = pm.getProcessAt(1);
    ASSERT_EQUAL(p2.id, 2, "P2 should be at index 1");
    ASSERT_EQUAL(p2.completionTime, 9, "P2 CT should be 9");
    ASSERT_EQUAL(p2.turnaroundTime, 8, "P2 TAT should be 8");
    ASSERT_EQUAL(p2.waitingTime, 5, "P2 WT should be 5");

    // P3: CT=16, TAT=14, WT=6
    Process p3 = pm.getProcessAt(2);
    ASSERT_EQUAL(p3.id, 3, "P3 should be at index 2");
    ASSERT_EQUAL(p3.completionTime, 16, "P3 CT should be 16");
    ASSERT_EQUAL(p3.turnaroundTime, 14, "P3 TAT should be 14");
    ASSERT_EQUAL(p3.waitingTime, 6, "P3 WT should be 6");

    // Averages: Avg TAT = 34 / 3 = 11.33, Avg WT = 18 / 3 = 6.00
    ASSERT_NEAR(rr.getAvgTurnaroundTime(), 11.33, 0.01, "Avg TAT should be 11.33");
    ASSERT_NEAR(rr.getAvgWaitingTime(), 6.00, 0.01, "Avg WT should be 6.00");
    return true;
}

bool testRRCpuIdleTime() {
    RoundRobin rr(2);
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 1); // P1: AT=0, BT=3
    pm.addProcess(2, 6, 2, 1); // P2: AT=6, BT=2 (CPU idle from t=3 to 6)

    rr.runSimulation(pm);

    ASSERT_EQUAL(rr.getTotalCpuTime(), 8, "Total CPU time should be 8");
    ASSERT_EQUAL(rr.getIdleTime(), 3, "Idle time should be 3 units");
    return true;
}

bool testRRDifferentQuantums() {
    RoundRobin rr1(1); // Quantum = 1
    ProcessManager pm1;
    pm1.addProcess(1, 0, 4, 1);
    pm1.addProcess(2, 0, 4, 1);

    rr1.runSimulation(pm1);

    // With Q=1, P1 and P2 alternate execution every 1 unit
    // P1 finishes at t=7, P2 finishes at t=8
    Process p1_q1 = pm1.getProcessAt(0);
    Process p2_q1 = pm1.getProcessAt(1);
    ASSERT_EQUAL(p1_q1.completionTime, 7, "With Q=1, P1 CT should be 7");
    ASSERT_EQUAL(p2_q1.completionTime, 8, "With Q=1, P2 CT should be 8");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     RUNNING ROUND ROBIN SCHEDULER TESTS\n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total = 3;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Round Robin Benchmark Workload (Q=2)", testRRSection24BenchmarkWorkload);
    runTest("Round Robin CPU Idle Time Handling", testRRCpuIdleTime);
    runTest("Round Robin Quantum Slice Behavior", testRRDifferentQuantums);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
