#ifndef SCHEDULER_BASE_H
#define SCHEDULER_BASE_H

#include "core/Process.h"
#include "core/ProcessManager.h"
#include "dsa/Stack.h"
#include "utils/SearchSort.h"
#include <string>
#include <iostream>
#include <iomanip>

// Base execution step entry stored in execution history stack
struct ExecutionStep {
    int pid;        // ID of process executed (-1 for CPU IDLE)
    int startTime;  // Time slice start time
    int endTime;    // Time slice end time

    ExecutionStep() : pid(-1), startTime(0), endTime(0) {}
    ExecutionStep(int p, int start, int end) : pid(p), startTime(start), endTime(end) {}
};

// Abstract Base Class defining common interface and metric capabilities for all CPU schedulers
class SchedulerBase {
protected:
    double avgWaitingTime;     // Calculated average waiting time
    double avgTurnaroundTime;  // Calculated average turnaround time
    int totalCpuTime;          // Total time units elapsed during simulation
    int idleTime;              // Total CPU idle time units
    Stack<ExecutionStep> executionHistory; // LIFO stack recording CPU execution slices

    // Calculates aggregate average waiting and turnaround metrics from completed processes
    void calculateAggregateMetrics(ProcessManager& processManager) {
        int count = processManager.getCount();
        if (count == 0) {
            avgWaitingTime = 0.0;
            avgTurnaroundTime = 0.0;
            return;
        }

        double totalWT = 0.0;
        double totalTAT = 0.0;
        Process* processes = processManager.getProcessesMutable();

        for (int i = 0; i < count; ++i) {
            processes[i].calculateMetrics();
            totalWT += processes[i].waitingTime;
            totalTAT += processes[i].turnaroundTime;
        }

        avgWaitingTime = totalWT / count;
        avgTurnaroundTime = totalTAT / count;
    }

public:
    SchedulerBase() 
        : avgWaitingTime(0.0), avgTurnaroundTime(0.0),
          totalCpuTime(0), idleTime(0) {}

    virtual ~SchedulerBase() {}

    // Disable copy operations for scheduler hierarchy
    SchedulerBase(const SchedulerBase&) = delete;
    SchedulerBase& operator=(const SchedulerBase&) = delete;

    // Pure virtual function executing CPU scheduling simulation on workload
    virtual void runSimulation(ProcessManager& processManager) = 0;

    // Pure virtual function returning scheduler algorithm name
    virtual std::string getAlgorithmName() const = 0;

    // Resets scheduler metrics and execution history
    virtual void reset() {
        avgWaitingTime = 0.0;
        avgTurnaroundTime = 0.0;
        totalCpuTime = 0;
        idleTime = 0;
        executionHistory.clear();
    }

    // Displays comprehensive simulation metrics summary
    virtual void printSummary(const ProcessManager& processManager) const {
        std::cout << "\n========================================\n";
        std::cout << "      SCHEDULER RESULTS: " << getAlgorithmName() << "\n";
        std::cout << "========================================\n";

        processManager.displayAll();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n--- Performance Metrics ---\n";
        std::cout << "Total Elapsed CPU Time : " << totalCpuTime << " units\n";
        std::cout << "Total CPU Idle Time    : " << idleTime << " units\n";
        std::cout << "Average Turnaround Time: " << avgTurnaroundTime << " units\n";
        std::cout << "Average Waiting Time   : " << avgWaitingTime << " units\n";
        std::cout << "========================================\n";
    }

    // Accessors
    double getAvgWaitingTime() const { return avgWaitingTime; }
    double getAvgTurnaroundTime() const { return avgTurnaroundTime; }
    int getTotalCpuTime() const { return totalCpuTime; }
    int getIdleTime() const { return idleTime; }
    const Stack<ExecutionStep>& getExecutionHistory() const { return executionHistory; }
};

#endif // SCHEDULER_BASE_H
