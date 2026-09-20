#include <iostream>
#include <cassert>
#include <cmath>
#include "schedulers/FCFS.h"
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

bool testFCFSSection24BenchmarkWorkload() {
    FCFS fcfs;
    ProcessManager pm;

    // Benchmark workload from Prompt Section 24
    pm.addProcess(1, 0, 5, 2); // P1: AT=0, BT=5
    pm.addProcess(2, 1, 3, 1); // P2: AT=1, BT=3
    pm.addProcess(3, 2, 8, 3); // P3: AT=2, BT=8

    fcfs.runSimulation(pm);

    ASSERT_EQUAL(fcfs.getTotalCpuTime(), 16, "Total CPU time should be 16");
    ASSERT_EQUAL(fcfs.getIdleTime(), 0, "Idle time should be 0");

    // P1 Metrics: CT=5, TAT=5, WT=0
    Process p1 = pm.getProcessAt(0);
    ASSERT_EQUAL(p1.id, 1, "P1 should be at index 0");
    ASSERT_EQUAL(p1.completionTime, 5, "P1 CT should be 5");
    ASSERT_EQUAL(p1.turnaroundTime, 5, "P1 TAT should be 5");
    ASSERT_EQUAL(p1.waitingTime, 0, "P1 WT should be 0");
    ASSERT_TRUE(p1.state == ProcessState::COMPLETED, "P1 state should be COMPLETED");

    // P2 Metrics: CT=8, TAT=7, WT=4
    Process p2 = pm.getProcessAt(1);
    ASSERT_EQUAL(p2.id, 2, "P2 should be at index 1");
    ASSERT_EQUAL(p2.completionTime, 8, "P2 CT should be 8");
    ASSERT_EQUAL(p2.turnaroundTime, 7, "P2 TAT should be 7");
    ASSERT_EQUAL(p2.waitingTime, 4, "P2 WT should be 4");

    // P3 Metrics: CT=16, TAT=14, WT=6
    Process p3 = pm.getProcessAt(2);
    ASSERT_EQUAL(p3.id, 3, "P3 should be at index 2");
    ASSERT_EQUAL(p3.completionTime, 16, "P3 CT should be 16");
    ASSERT_EQUAL(p3.turnaroundTime, 14, "P3 TAT should be 14");
    ASSERT_EQUAL(p3.waitingTime, 6, "P3 WT should be 6");

    // Aggregate averages: Avg TAT = 26 / 3 = 8.67, Avg WT = 10 / 3 = 3.33
    ASSERT_NEAR(fcfs.getAvgTurnaroundTime(), 8.67, 0.01, "Avg TAT should be 8.67");
    ASSERT_NEAR(fcfs.getAvgWaitingTime(), 3.33, 0.01, "Avg WT should be 3.33");
    return true;
}

bool testFCFSCpuIdleTime() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 1); // P1: AT=0, BT=3 -> CT=3
    pm.addProcess(2, 5, 2, 1); // P2: AT=5, BT=2 -> CT=7 (CPU idle from t=3 to 5)

    fcfs.runSimulation(pm);

    ASSERT_EQUAL(fcfs.getTotalCpuTime(), 7, "Total CPU time should be 7");
    ASSERT_EQUAL(fcfs.getIdleTime(), 2, "Idle time should be 2 units");

    Process p2 = pm.getProcessAt(1);
    ASSERT_EQUAL(p2.completionTime, 7, "P2 completion time should be 7");
    ASSERT_EQUAL(p2.waitingTime, 0, "P2 waiting time should be 0");
    return true;
}

bool testFCFSOutOfOrderArrivals() {
    FCFS fcfs;
    ProcessManager pm;

    // Out of order: P2 added before P1
    pm.addProcess(2, 2, 4, 1); // AT=2
    pm.addProcess(1, 0, 3, 1); // AT=0

    fcfs.runSimulation(pm);

    // P1 (AT=0) must execute first, then P2 (AT=2)
    Process pFirst = pm.getProcessAt(0);
    Process pSecond = pm.getProcessAt(1);

    ASSERT_EQUAL(pFirst.id, 1, "P1 should execute first");
    ASSERT_EQUAL(pSecond.id, 2, "P2 should execute second");

    ASSERT_EQUAL(pFirst.completionTime, 3, "P1 CT should be 3");
    ASSERT_EQUAL(pSecond.completionTime, 7, "P2 CT should be 7");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "       RUNNING FCFS SCHEDULER TESTS     \n";
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

    runTest("FCFS Benchmark Workload (Section 24)", testFCFSSection24BenchmarkWorkload);
    runTest("FCFS CPU Idle Time Handling", testFCFSCpuIdleTime);
    runTest("FCFS Out-of-Order Arrival Pre-Sorting", testFCFSOutOfOrderArrivals);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
