#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>

// Represents the operational lifecycle state of a process
enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    COMPLETED
};

// Helper function to convert ProcessState enum to string representation
std::string processStateToString(ProcessState state);

class Process {
public:
    int id;             // Process Identifier (PID)
    int arrivalTime;    // Time at which process arrives in ready queue
    int burstTime;      // Total CPU time required by process
    int priority;       // Priority level (lower integer = higher priority, e.g., 1 is highest)
    int remainingTime;  // CPU burst time remaining for execution
    int completionTime; // Time at which process finishes execution
    int turnaroundTime;// Total time from arrival to completion (CT - AT)
    int waitingTime;   // Total time spent waiting in ready queue (TAT - BT)
    ProcessState state; // Current lifecycle state of the process

    // Default constructor
    Process();

    // Parameterized constructor
    Process(int pid, int arrival, int burst, int prio = 1);

    // Recalculates Turnaround Time and Waiting Time based on Completion Time
    void calculateMetrics();

    // Resets remaining time and metrics for a fresh simulation run
    void reset();

    // Pretty-print single process metrics in tabular row format
    void printProcessDetails() const;
};

#endif // PROCESS_H
