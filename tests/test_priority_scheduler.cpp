#include <iostream>
#include <cassert>
#include <cmath>
#include "schedulers/PriorityScheduler.h"
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

bool testPriorityBenchmarkWorkload() {
    PriorityScheduler prioritySched;
    ProcessManager pm;

    // Benchmark workload from Prompt Section 24
    pm.addProcess(1, 0, 5, 2); // P1: AT=0, BT=5, Prio=2
    pm.addProcess(2, 1, 3, 1); // P2: AT=1, BT=3, Prio=1
    pm.addProcess(3, 2, 8, 3); // P3: AT=2, BT=8, Prio=3

    prioritySched.runSimulation(pm);

    ASSERT_EQUAL(prioritySched.getTotalCpuTime(), 16, "Total CPU time should be 16");
    ASSERT_EQUAL(prioritySched.getIdleTime(), 0, "Idle time should be 0");

    // Non-preemptive execution: P1 runs t=0..5, P2 runs t=5..8, P3 runs t=8..16
    Process p1 = pm.getProcessAt(0);
    ASSERT_EQUAL(p1.id, 1, "P1 should be at index 0");
    ASSERT_EQUAL(p1.completionTime, 5, "P1 CT should be 5");
    ASSERT_EQUAL(p1.turnaroundTime, 5, "P1 TAT should be 5");
    ASSERT_EQUAL(p1.waitingTime, 0, "P1 WT should be 0");

    Process p2 = pm.getProcessAt(1);
    ASSERT_EQUAL(p2.id, 2, "P2 should be at index 1");
    ASSERT_EQUAL(p2.completionTime, 8, "P2 CT should be 8");
    ASSERT_EQUAL(p2.turnaroundTime, 7, "P2 TAT should be 7");
    ASSERT_EQUAL(p2.waitingTime, 4, "P2 WT should be 4");

    Process p3 = pm.getProcessAt(2);
    ASSERT_EQUAL(p3.id, 3, "P3 should be at index 2");
    ASSERT_EQUAL(p3.completionTime, 16, "P3 CT should be 16");
    ASSERT_EQUAL(p3.turnaroundTime, 14, "P3 TAT should be 14");
    ASSERT_EQUAL(p3.waitingTime, 6, "P3 WT should be 6");

    // Averages: Avg TAT = 26 / 3 = 8.67, Avg WT = 10 / 3 = 3.33
    ASSERT_NEAR(prioritySched.getAvgTurnaroundTime(), 8.67, 0.01, "Avg TAT should be 8.67");
    ASSERT_NEAR(prioritySched.getAvgWaitingTime(), 3.33, 0.01, "Avg WT should be 3.33");
    return true;
}

bool testPriorityDynamicArrivalSelection() {
    PriorityScheduler prioritySched;
    ProcessManager pm;

    pm.addProcess(1, 0, 10, 3); // P1: AT=0, BT=10, Prio=3
    pm.addProcess(2, 2, 2, 1);  // P2: AT=2, BT=2, Prio=1 (Arrives while P1 is running)
    pm.addProcess(3, 2, 4, 2);  // P3: AT=2, BT=4, Prio=2 (Arrives while P1 is running)

    prioritySched.runSimulation(pm);

    // Non-preemptive rule: P1 continues running until t=10.
    // At t=10, P2 (Prio 1) is selected over P3 (Prio 2). P2 finishes at t=12. P3 finishes at t=16.
    Process p1 = pm.getProcessAt(0);
    Process p2 = pm.getProcessAt(1);
    Process p3 = pm.getProcessAt(2);

    ASSERT_EQUAL(p1.completionTime, 10, "P1 CT should be 10 (Non-preempted)");
    ASSERT_EQUAL(p2.completionTime, 12, "P2 CT should be 12 (Selected first from Heap)");
    ASSERT_EQUAL(p3.completionTime, 16, "P3 CT should be 16");
    return true;
}

bool testPriorityCpuIdleTime() {
    PriorityScheduler prioritySched;
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 2); // P1: AT=0, BT=3, Prio=2
    pm.addProcess(2, 6, 2, 1); // P2: AT=6, BT=2, Prio=1 (CPU idle from t=3 to 6)

    prioritySched.runSimulation(pm);

    ASSERT_EQUAL(prioritySched.getTotalCpuTime(), 8, "Total CPU time should be 8");
    ASSERT_EQUAL(prioritySched.getIdleTime(), 3, "Idle time should be 3 units");
    return true;
}

bool testAgingMechanism() {
    PriorityScheduler sched(5); // agingInterval = 5
    ProcessManager pm;
    
    pm.addProcess(1, 0, 10, 1);  // P1 runs t=0..10
    pm.addProcess(2, 1, 10, 5);  // P2 low priority
    pm.addProcess(3, 2, 10, 3);  // P3 med priority
    pm.addProcess(4, 11, 10, 3); // P4 med priority arriving later

    sched.runSimulation(pm);

    // Timeline with aging:
    // t=0: P1 starts.
    // t=10: P1 finishes. P2(wait=9, prio=5-1=4), P3(wait=8, prio=3-1=2). P3 selected.
    // t=20: P3 finishes. P2(wait=19, prio=5-3=2), P4(wait=9, prio=3-1=2). 
    //       Both prio=2. P2 arrived first (1 < 11). P2 selected.
    // t=30: P2 finishes. P4(wait=19, prio=3-3=1 -> 1). P4 selected.
    // t=40: P4 finishes.
    ASSERT_EQUAL(pm.getProcessAt(0).completionTime, 10, "P1 finishes at 10");
    ASSERT_EQUAL(pm.getProcessAt(2).completionTime, 20, "P3 finishes at 20");
    ASSERT_EQUAL(pm.getProcessAt(1).completionTime, 30, "P2 finishes at 30 due to aging");
    ASSERT_EQUAL(pm.getProcessAt(3).completionTime, 40, "P4 finishes at 40");

    // Verify without aging, order is P1, P3, P4, P2 (P2 starves)
    PriorityScheduler schedNoAging(9999);
    ProcessManager pmNoAging;
    pmNoAging.addProcess(1, 0, 10, 1); 
    pmNoAging.addProcess(2, 1, 10, 5); 
    pmNoAging.addProcess(3, 2, 10, 3);
    pmNoAging.addProcess(4, 11, 10, 3);
    schedNoAging.runSimulation(pmNoAging);
    
    ASSERT_EQUAL(pmNoAging.getProcessAt(0).completionTime, 10, "No Aging P1 finishes at 10");
    ASSERT_EQUAL(pmNoAging.getProcessAt(2).completionTime, 20, "No Aging P3 finishes at 20");
    ASSERT_EQUAL(pmNoAging.getProcessAt(3).completionTime, 30, "No Aging P4 bypasses P2");
    ASSERT_EQUAL(pmNoAging.getProcessAt(1).completionTime, 40, "No Aging P2 starves to end");

    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   RUNNING PRIORITY SCHEDULER TESTS     \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total = 4;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Priority Benchmark Workload (Section 24)", testPriorityBenchmarkWorkload);
    runTest("Priority Non-Preemptive Execution & MinHeap Selection", testPriorityDynamicArrivalSelection);
    runTest("Priority CPU Idle Time Handling", testPriorityCpuIdleTime);
    runTest("Aging Mechanism Prevents Starvation", testAgingMechanism);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
