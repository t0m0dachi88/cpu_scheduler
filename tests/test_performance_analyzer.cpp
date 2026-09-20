#include <iostream>
#include <cassert>
#include <cmath>
#include "utils/PerformanceAnalyzer.h"
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

#define ASSERT_NEAR(actual, expected, tol, message) \
    do { \
        if (std::abs((actual) - (expected)) > (tol)) { \
            std::cerr << "[FAIL]: " << message \
                      << " (Expected: " << (expected) << ", Got: " << (actual) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

// TC-001: Verify analyze() runs all 3 schedulers and records 3 results
bool testAnalyzeRecordsThreeResults() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    pa.analyze(pm);

    ASSERT_EQUAL(pa.getResultCount(), 3,
                 "analyze() should record 3 algorithm results (FCFS, RR, Priority)");
    return true;
}

// TC-002: Verify FCFS result metrics match Section 24 expected values
bool testFCFSResultMetrics() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    pa.analyze(pm);

    const AlgorithmResult& fcfsResult = pa.getResult(0); // FCFS is index 0

    ASSERT_NEAR(fcfsResult.avgTurnaround, 8.67, 0.01,
                "FCFS Avg TAT should be 8.67");
    ASSERT_NEAR(fcfsResult.avgWaiting, 3.33, 0.01,
                "FCFS Avg WT should be 3.33");
    ASSERT_EQUAL(fcfsResult.totalCpuTime, 16,
                 "FCFS total CPU time should be 16");
    ASSERT_EQUAL(fcfsResult.idleTime, 0,
                 "FCFS idle time should be 0");
    ASSERT_NEAR(fcfsResult.cpuUtilization, 100.0, 0.01,
                "FCFS CPU utilization should be 100%");
    return true;
}

// TC-003: Verify Round Robin result metrics match Section 24 expected values
bool testRoundRobinResultMetrics() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    pa.analyze(pm);

    const AlgorithmResult& rrResult = pa.getResult(1); // RR is index 1

    ASSERT_NEAR(rrResult.avgTurnaround, 11.33, 0.01,
                "RR Avg TAT should be 11.33");
    ASSERT_NEAR(rrResult.avgWaiting, 6.00, 0.01,
                "RR Avg WT should be 6.00");
    ASSERT_EQUAL(rrResult.totalCpuTime, 16,
                 "RR total CPU time should be 16");
    ASSERT_EQUAL(rrResult.idleTime, 0,
                 "RR idle time should be 0");
    return true;
}

// TC-004: Verify Priority Scheduler result metrics
bool testPriorityResultMetrics() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    pa.analyze(pm);

    const AlgorithmResult& prioResult = pa.getResult(2); // Priority is index 2

    ASSERT_NEAR(prioResult.avgTurnaround, 8.67, 0.01,
                "Priority Avg TAT should be 8.67");
    ASSERT_NEAR(prioResult.avgWaiting, 3.33, 0.01,
                "Priority Avg WT should be 3.33");
    ASSERT_EQUAL(prioResult.totalCpuTime, 16,
                 "Priority total CPU time should be 16");
    return true;
}

// TC-005: Verify CPU utilization calculation for workload with idle gap
bool testCpuUtilizationWithIdleGap() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 1); // P1: t=0..3
    pm.addProcess(2, 6, 2, 1); // P2: t=6..8; idle gap t=3..6

    pa.analyze(pm);

    // FCFS result (index 0): total=8, idle=3 → utilization = 5/8 * 100 = 62.5%
    const AlgorithmResult& fcfsResult = pa.getResult(0);
    ASSERT_EQUAL(fcfsResult.totalCpuTime, 8,
                 "CPU time with idle gap should be 8");
    ASSERT_EQUAL(fcfsResult.idleTime, 3,
                 "Idle time should be 3 units");
    ASSERT_NEAR(fcfsResult.cpuUtilization, 62.5, 0.01,
                "CPU utilization with idle gap should be 62.5%");
    return true;
}

// TC-006: Verify empty workload is handled gracefully (no crash)
bool testEmptyWorkloadHandling() {
    PerformanceAnalyzer pa;
    ProcessManager pm;
    // No processes added

    pa.analyze(pm); // Should not crash

    ASSERT_EQUAL(pa.getResultCount(), 0,
                 "Empty workload should produce 0 results");
    return true;
}

// TC-007: Verify printComparisonTable() produces output without crashing
bool testPrintComparisonTableNoCrash() {
    PerformanceAnalyzer pa;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    pa.analyze(pm);
    pa.printComparisonTable(); // Should not throw or crash

    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   RUNNING PERFORMANCE ANALYZER TESTS  \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total  = 7;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Analyze Records 3 Algorithm Results",      testAnalyzeRecordsThreeResults);
    runTest("FCFS Result Metrics (Section 24)",         testFCFSResultMetrics);
    runTest("Round Robin Result Metrics (Section 24)",  testRoundRobinResultMetrics);
    runTest("Priority Scheduler Result Metrics",        testPriorityResultMetrics);
    runTest("CPU Utilization With Idle Gap (62.5%)",    testCpuUtilizationWithIdleGap);
    runTest("Empty Workload Graceful Handling",         testEmptyWorkloadHandling);
    runTest("printComparisonTable No Crash",            testPrintComparisonTableNoCrash);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
