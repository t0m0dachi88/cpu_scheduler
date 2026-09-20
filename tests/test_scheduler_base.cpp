#include <iostream>
#include <cassert>
#include <string>
#include "schedulers/SchedulerBase.h"
#include "core/ProcessManager.h"
#include "utils/SearchSort.h"

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

// Mock Scheduler for testing base class framework behavior
class MockScheduler : public SchedulerBase {
public:
    std::string getAlgorithmName() const override {
        return "Mock Scheduler";
    }

    void runSimulation(ProcessManager& processManager) override {
        reset();
        
        // Enforce chronological sorting by Arrival Time as required by architecture
        SearchSort::sortByArrivalTime(processManager.getProcessesMutable(), processManager.getCount());

        Process* processes = processManager.getProcessesMutable();
        int count = processManager.getCount();
        int currentTime = 0;

        for (int i = 0; i < count; ++i) {
            if (currentTime < processes[i].arrivalTime) {
                idleTime += (processes[i].arrivalTime - currentTime);
                executionHistory.push(ExecutionStep(-1, currentTime, processes[i].arrivalTime));
                currentTime = processes[i].arrivalTime;
            }

            int start = currentTime;
            currentTime += processes[i].burstTime;
            processes[i].completionTime = currentTime;
            processes[i].calculateMetrics();
            processes[i].state = ProcessState::COMPLETED;

            executionHistory.push(ExecutionStep(processes[i].id, start, currentTime));
        }

        totalCpuTime = currentTime;
        calculateAggregateMetrics(processManager);
    }
};

bool testSchedulerBaseInterface() {
    MockScheduler mock;
    SchedulerBase* scheduler = &mock;

    ASSERT_EQUAL(scheduler->getAlgorithmName(), "Mock Scheduler", "Polymorphic algorithm name mismatch");
    return true;
}

bool testSchedulerBaseMetricAggregation() {
    MockScheduler mock;
    ProcessManager pm;
    
    // P1: AT=0, BT=5 -> CT=5, TAT=5, WT=0
    // P2: AT=1, BT=3 -> CT=8, TAT=7, WT=4
    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);

    mock.runSimulation(pm);

    ASSERT_EQUAL(mock.getTotalCpuTime(), 8, "Total CPU time should be 8");
    ASSERT_EQUAL(mock.getIdleTime(), 0, "Idle time should be 0");
    
    // Avg TAT = (5 + 7) / 2 = 6.0
    // Avg WT = (0 + 4) / 2 = 2.0
    ASSERT_EQUAL(mock.getAvgTurnaroundTime(), 6.0, "Average TAT mismatch");
    ASSERT_EQUAL(mock.getAvgWaitingTime(), 2.0, "Average WT mismatch");

    ASSERT_EQUAL(mock.getExecutionHistory().size(), 2, "Execution history stack size should be 2");
    return true;
}

bool testSchedulerBaseChronologicalArrivalSorting() {
    MockScheduler mock;
    ProcessManager pm;

    // Added out of chronological order: P2(AT=5) then P1(AT=0)
    pm.addProcess(2, 5, 2, 1);
    pm.addProcess(1, 0, 4, 2);

    mock.runSimulation(pm);

    // After sorting, P1 (AT=0) should execute first, then idle for 1 unit (4 to 5), then P2 (AT=5 to 7)
    Process pFirst = pm.getProcessAt(0);
    Process pSecond = pm.getProcessAt(1);

    ASSERT_EQUAL(pFirst.id, 1, "P1 should be first process in array after arrival sort");
    ASSERT_EQUAL(pSecond.id, 2, "P2 should be second process in array after arrival sort");

    ASSERT_EQUAL(mock.getTotalCpuTime(), 7, "Total CPU time should be 7");
    ASSERT_EQUAL(mock.getIdleTime(), 1, "Idle time should be 1 unit between t=4 and t=5");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   RUNNING SCHEDULER BASE UNIT TESTS   \n";
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

    runTest("SchedulerBase Interface & Polymorphism", testSchedulerBaseInterface);
    runTest("SchedulerBase Metric Aggregation & History Stack", testSchedulerBaseMetricAggregation);
    runTest("SchedulerBase Chronological Arrival Sorting", testSchedulerBaseChronologicalArrivalSorting);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
