#ifndef STATE_TRACKER_H
#define STATE_TRACKER_H

#include "core/Process.h"
#include <string>
#include <iostream>

// Records a single process state transition event
struct StateTransition {
    int pid;                 // Process ID that transitioned
    ProcessState fromState;  // State before transition
    ProcessState toState;    // State after transition
    int timestamp;           // Simulation time at which the transition occurred

    StateTransition()
        : pid(0), fromState(ProcessState::NEW), toState(ProcessState::NEW), timestamp(0) {}

    StateTransition(int p, ProcessState from, ProcessState to, int time)
        : pid(p), fromState(from), toState(to), timestamp(time) {}
};

// Tracks and logs process state transitions during CPU scheduling simulations.
// Uses a fixed-size array (no STL containers) to store chronological transition events.
class StateTracker {
public:
    static const int MAX_TRANSITIONS = 512;

    StateTracker();

    // Records a state transition for a process.
    // Validates that the transition follows the legal lifecycle:
    //   NEW -> READY -> RUNNING -> COMPLETED
    //                RUNNING -> READY (preemption / quantum expiry)
    // Returns true if transition was recorded, false if log is full or invalid.
    bool recordTransition(int pid, ProcessState from, ProcessState to, int timestamp);

    // Convenience: transitions the process's state field and records the event
    bool transitionProcess(Process& proc, ProcessState newState, int timestamp);

    // Returns the number of recorded transitions
    int getCount() const { return transitionCount; }

    // Returns a const pointer to the transitions array
    const StateTransition* getTransitions() const { return transitions; }

    // Returns a specific transition by index
    const StateTransition& getTransition(int index) const;

    // Clears all recorded transitions
    void clear();

    // Validates that a state transition is legal
    static bool isValidTransition(ProcessState from, ProcessState to);

    // Renders a formatted state transition log to stdout
    void printTransitionLog() const;

    // Returns the number of transitions for a specific process
    int getTransitionCountForProcess(int pid) const;

private:
    StateTransition transitions[MAX_TRANSITIONS]; // Fixed-size transition log
    int transitionCount;                           // Number of valid entries
};

#endif // STATE_TRACKER_H
