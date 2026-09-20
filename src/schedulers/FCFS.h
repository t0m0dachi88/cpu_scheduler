#ifndef FCFS_H
#define FCFS_H

#include "schedulers/SchedulerBase.h"
#include "dsa/CustomQueue.h"

// First-Come, First-Served (FCFS) CPU Scheduler implementation
class FCFS : public SchedulerBase {
public:
    FCFS() = default;
    ~FCFS() override = default;

    // Returns human-readable algorithm name
    std::string getAlgorithmName() const override {
        return "First-Come First-Served (FCFS)";
    }

    // Executes non-preemptive FCFS CPU simulation on the given workload
    void runSimulation(ProcessManager& processManager) override;
};

#endif // FCFS_H
