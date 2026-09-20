#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "schedulers/SchedulerBase.h"
#include "dsa/CircularQueue.h"

// Round Robin (RR) CPU Scheduler implementation using CircularQueue
class RoundRobin : public SchedulerBase {
private:
    int timeQuantum; // Configurable Time Quantum parameter (default = 2)

public:
    explicit RoundRobin(int quantum = 2);
    ~RoundRobin() override = default;

    // Time Quantum getter and setter
    int getTimeQuantum() const { return timeQuantum; }
    void setTimeQuantum(int quantum) {
        if (quantum > 0) {
            timeQuantum = quantum;
        }
    }

    // Returns algorithm name including current time quantum configuration
    std::string getAlgorithmName() const override {
        return "Round Robin (RR, Quantum = " + std::to_string(timeQuantum) + ")";
    }

    // Executes preemptive Round Robin CPU simulation on the given workload
    void runSimulation(ProcessManager& processManager) override;
};

#endif // ROUND_ROBIN_H
