#include <iostream>
#include <cassert>
#include <string>
#include "utils/StateTracker.h"
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

// TC-001: Verify valid lifecycle transitions are accepted
bool testValidTransitions() {
    ASSERT_TRUE(StateTracker::isValidTransition(ProcessState::NEW,     ProcessState::READY),
                "NEW -> READY should be valid");
    ASSERT_TRUE(StateTracker::isValidTransition(ProcessState::READY,   ProcessState::RUNNING),
                "READY -> RUNNING should be valid");
    ASSERT_TRUE(StateTracker::isValidTransition(ProcessState::RUNNING, ProcessState::COMPLETED),
                "RUNNING -> COMPLETED should be valid");
    ASSERT_TRUE(StateTracker::isValidTransition(ProcessState::RUNNING, ProcessState::READY),
                "RUNNING -> READY (preemption) should be valid");
    return true;
}

// TC-002: Verify invalid transitions are rejected
bool testInvalidTransitions() {
    ASSERT_TRUE(!StateTracker::isValidTransition(ProcessState::NEW,       ProcessState::RUNNING),
                "NEW -> RUNNING should be invalid (must go through READY)");
    ASSERT_TRUE(!StateTracker::isValidTransition(ProcessState::NEW,       ProcessState::COMPLETED),
                "NEW -> COMPLETED should be invalid");
    ASSERT_TRUE(!StateTracker::isValidTransition(ProcessState::READY,     ProcessState::COMPLETED),
                "READY -> COMPLETED should be invalid (must go through RUNNING)");
    ASSERT_TRUE(!StateTracker::isValidTransition(ProcessState::COMPLETED, ProcessState::NEW),
                "COMPLETED -> NEW should be invalid");
    ASSERT_TRUE(!StateTracker::isValidTransition(ProcessState::READY,     ProcessState::NEW),
                "READY -> NEW should be invalid");
    return true;
}

// TC-003: Verify transitionProcess() updates process state and records event
bool testTransitionProcessUpdatesState() {
    StateTracker tracker;
    Process p(1, 0, 5, 1);

    ASSERT_TRUE(p.state == ProcessState::NEW, "Process should start as NEW");

    bool ok = tracker.transitionProcess(p, ProcessState::READY, 0);
    ASSERT_TRUE(ok, "NEW -> READY transition should succeed");
    ASSERT_TRUE(p.state == ProcessState::READY, "Process state should now be READY");

    ok = tracker.transitionProcess(p, ProcessState::RUNNING, 0);
    ASSERT_TRUE(ok, "READY -> RUNNING transition should succeed");
    ASSERT_TRUE(p.state == ProcessState::RUNNING, "Process state should now be RUNNING");

    ok = tracker.transitionProcess(p, ProcessState::COMPLETED, 5);
    ASSERT_TRUE(ok, "RUNNING -> COMPLETED transition should succeed");
    ASSERT_TRUE(p.state == ProcessState::COMPLETED, "Process state should now be COMPLETED");

    ASSERT_EQUAL(tracker.getCount(), 3, "Should have recorded 3 transitions");

    // Verify first transition details
    const StateTransition& t0 = tracker.getTransition(0);
    ASSERT_EQUAL(t0.pid, 1, "First transition PID should be 1");
    ASSERT_TRUE(t0.fromState == ProcessState::NEW, "First from state should be NEW");
    ASSERT_TRUE(t0.toState == ProcessState::READY, "First to state should be READY");

    return true;
}

// TC-004: Verify FCFS records correct transitions for 3-process workload
bool testFCFSStateTransitions() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 5, 2);
    pm.addProcess(2, 1, 3, 1);
    pm.addProcess(3, 2, 8, 3);

    fcfs.runSimulation(pm);

    const StateTracker& tracker = fcfs.getStateTracker();

    // Each non-preemptive process: NEW->READY, READY->RUNNING, RUNNING->COMPLETED = 3 transitions
    // 3 processes × 3 transitions = 9 total
    ASSERT_EQUAL(tracker.getCount(), 9,
                 "FCFS with 3 processes should produce 9 transitions (3 per process)");

    // Each process should have exactly 3 transitions
    ASSERT_EQUAL(tracker.getTransitionCountForProcess(1), 3, "P1 should have 3 transitions");
    ASSERT_EQUAL(tracker.getTransitionCountForProcess(2), 3, "P2 should have 3 transitions");
    ASSERT_EQUAL(tracker.getTransitionCountForProcess(3), 3, "P3 should have 3 transitions");

    return true;
}

// TC-005: Verify Round Robin records preemption (RUNNING -> READY) transitions
bool testRoundRobinPreemptionTransitions() {
    RoundRobin rr(2);
    ProcessManager pm;

    pm.addProcess(1, 0, 4, 1); // P1: needs 2 quanta
    pm.addProcess(2, 0, 4, 1); // P2: needs 2 quanta

    rr.runSimulation(pm);

    const StateTracker& tracker = rr.getStateTracker();

    // P1: NEW->READY(t=0), READY->RUNNING(t=0), RUNNING->READY(t=2) [preempted],
    //     READY->RUNNING(t=4), RUNNING->COMPLETED(t=6) = 5 transitions
    // P2: NEW->READY(t=0), READY->RUNNING(t=2), RUNNING->READY(t=4) [preempted],
    //     READY->RUNNING(t=6), RUNNING->COMPLETED(t=8) = 5 transitions
    // Total = 10
    ASSERT_EQUAL(tracker.getCount(), 10,
                 "RR Q=2 with two 4-BT processes should produce 10 transitions");

    ASSERT_EQUAL(tracker.getTransitionCountForProcess(1), 5, "P1 should have 5 transitions");
    ASSERT_EQUAL(tracker.getTransitionCountForProcess(2), 5, "P2 should have 5 transitions");

    // Verify a RUNNING->READY preemption exists
    bool foundPreemption = false;
    for (int i = 0; i < tracker.getCount(); ++i) {
        const StateTransition& t = tracker.getTransition(i);
        if (t.fromState == ProcessState::RUNNING && t.toState == ProcessState::READY) {
            foundPreemption = true;
            break;
        }
    }
    ASSERT_TRUE(foundPreemption, "Should have at least one RUNNING->READY preemption transition");

    return true;
}

// TC-006: Verify printTransitionLog renders without crashing
bool testPrintTransitionLogNoCrash() {
    FCFS fcfs;
    ProcessManager pm;

    pm.addProcess(1, 0, 3, 1);
    pm.addProcess(2, 3, 2, 1);

    fcfs.runSimulation(pm);

    // Should not throw or crash
    fcfs.getStateTracker().printTransitionLog();

    return true;
}

// TC-007: Verify invalid transition is rejected by transitionProcess
bool testInvalidTransitionRejected() {
    StateTracker tracker;
    Process p(1, 0, 5, 1);
    // p starts as NEW; attempting NEW -> RUNNING (skipping READY) should fail
    bool ok = tracker.transitionProcess(p, ProcessState::RUNNING, 0);
    ASSERT_TRUE(!ok, "NEW -> RUNNING should be rejected");
    ASSERT_TRUE(p.state == ProcessState::NEW, "Process state should remain NEW after rejection");
    ASSERT_EQUAL(tracker.getCount(), 0, "No transition should be recorded after rejection");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "    RUNNING STATE TRACKER UNIT TESTS    \n";
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

    runTest("Valid Lifecycle Transitions",                  testValidTransitions);
    runTest("Invalid Transitions Rejected",                testInvalidTransitions);
    runTest("transitionProcess Updates State & Records",   testTransitionProcessUpdatesState);
    runTest("FCFS State Transitions (9 events, 3/proc)",   testFCFSStateTransitions);
    runTest("Round Robin Preemption Transitions",          testRoundRobinPreemptionTransitions);
    runTest("printTransitionLog No Crash",                 testPrintTransitionLogNoCrash);
    runTest("Invalid Transition Rejection",                testInvalidTransitionRejected);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
