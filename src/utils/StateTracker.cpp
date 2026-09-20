#include "utils/StateTracker.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

StateTracker::StateTracker() : transitionCount(0) {}

// ─── Transition Validation ───────────────────────────────────────────────────

bool StateTracker::isValidTransition(ProcessState from, ProcessState to) {
    // Legal transitions in a CPU scheduling lifecycle:
    //   NEW       -> READY      (process enters ready queue)
    //   READY     -> RUNNING    (CPU dispatches process)
    //   RUNNING   -> COMPLETED  (process finishes execution)
    //   RUNNING   -> READY      (preemption: quantum expired or higher priority arrived)
    if (from == ProcessState::NEW       && to == ProcessState::READY)     return true;
    if (from == ProcessState::READY     && to == ProcessState::RUNNING)   return true;
    if (from == ProcessState::RUNNING   && to == ProcessState::COMPLETED) return true;
    if (from == ProcessState::RUNNING   && to == ProcessState::READY)     return true;
    return false;
}

// ─── Recording ───────────────────────────────────────────────────────────────

bool StateTracker::recordTransition(int pid, ProcessState from, ProcessState to, int timestamp) {
    if (transitionCount >= MAX_TRANSITIONS) return false;
    if (!isValidTransition(from, to)) return false;

    transitions[transitionCount] = StateTransition(pid, from, to, timestamp);
    transitionCount++;
    return true;
}

bool StateTracker::transitionProcess(Process& proc, ProcessState newState, int timestamp) {
    ProcessState oldState = proc.state;
    if (!isValidTransition(oldState, newState)) return false;
    if (transitionCount >= MAX_TRANSITIONS) return false;

    transitions[transitionCount] = StateTransition(proc.id, oldState, newState, timestamp);
    transitionCount++;
    proc.state = newState;
    return true;
}

// ─── Accessors ───────────────────────────────────────────────────────────────

const StateTransition& StateTracker::getTransition(int index) const {
    if (index < 0 || index >= transitionCount) {
        throw std::out_of_range("StateTracker: transition index out of range.");
    }
    return transitions[index];
}

void StateTracker::clear() {
    transitionCount = 0;
}

int StateTracker::getTransitionCountForProcess(int pid) const {
    int count = 0;
    for (int i = 0; i < transitionCount; ++i) {
        if (transitions[i].pid == pid) {
            count++;
        }
    }
    return count;
}

// ─── Display ─────────────────────────────────────────────────────────────────

void StateTracker::printTransitionLog() const {
    if (transitionCount == 0) {
        std::cout << "  [StateTracker] No transitions recorded.\n";
        return;
    }

    std::cout << "\n  Process State Transition Log:\n";
    std::cout << "  +-------+------+----------------+----------------+\n";
    std::cout << "  |  Time | PID  | From State     | To State       |\n";
    std::cout << "  +-------+------+----------------+----------------+\n";

    for (int i = 0; i < transitionCount; ++i) {
        const StateTransition& t = transitions[i];
        std::cout << "  | " << std::setw(5) << t.timestamp
                  << " | P" << std::left << std::setw(3) << t.pid << std::right
                  << " | " << std::setw(14) << std::left << processStateToString(t.fromState)
                  << std::right
                  << " | " << std::setw(14) << std::left << processStateToString(t.toState)
                  << std::right << " |\n";
    }

    std::cout << "  +-------+------+----------------+----------------+\n";
    std::cout << "  Total Transitions: " << transitionCount << "\n\n";
}
