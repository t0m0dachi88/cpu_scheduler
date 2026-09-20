#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "schedulers/SchedulerBase.h"
#include "dsa/MinHeap.h"

// Priority comparator function for MinHeap process pointer ordering
// Rule: Lower priority number = higher priority; tie-break by arrival time, then PID.
bool prioritySchedulerComparator(Process* const& a, Process* const& b);

// Non-preemptive Priority CPU Scheduler implementation using MinHeap
class PriorityScheduler : public SchedulerBase {
private:
    int agingInterval; // Time units a process must wait for its priority to increase (decrease in int value)

public:
    explicit PriorityScheduler(int aging = 5);
    ~PriorityScheduler() override = default;

    // Returns algorithm name including aging configuration
    std::string getAlgorithmName() const override {
        return "Priority Scheduling (Non-preemptive, Aging=" + std::to_string(agingInterval) + ")";
    }

    // Executes non-preemptive Priority CPU simulation on workload using MinHeap
    void runSimulation(ProcessManager& processManager) override;
};

#endif // PRIORITY_SCHEDULER_H
