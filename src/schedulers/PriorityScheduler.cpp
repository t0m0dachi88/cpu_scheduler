#include "schedulers/PriorityScheduler.h"
#include "utils/SearchSort.h"

bool prioritySchedulerComparator(Process* const& a, Process* const& b) {
    if (a->priority != b->priority) {
        return a->priority < b->priority; // Lower integer = higher priority
    }
    if (a->arrivalTime != b->arrivalTime) {
        return a->arrivalTime < b->arrivalTime; // Earlier arrival first
    }
    return a->id < b->id; // Lower PID first
}

void PriorityScheduler::runSimulation(ProcessManager& processManager) {
    reset();

    int count = processManager.getCount();
    if (count == 0) return;

    // Enforce chronological sorting by Arrival Time
    SearchSort::sortByArrivalTime(processManager.getProcessesMutable(), count);
    processManager.resetAll();

    Process* processes = processManager.getProcessesMutable();
    MinHeap<Process*> readyHeap(count > 10 ? count : 10, prioritySchedulerComparator);

    int currentTime = 0;
    int completedCount = 0;
    int arrivalIdx = 0;

    while (completedCount < count) {
        // Enqueue all processes that have arrived by currentTime into MinHeap
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            processes[arrivalIdx].state = ProcessState::READY;
            readyHeap.insert(&processes[arrivalIdx]);
            arrivalIdx++;
        }

        // If readyHeap is empty but processes remain, CPU is idle
        if (readyHeap.isEmpty()) {
            if (arrivalIdx < count) {
                int idleStart = currentTime;
                currentTime = processes[arrivalIdx].arrivalTime;
                idleTime += (currentTime - idleStart);
                executionHistory.push(ExecutionStep(-1, idleStart, currentTime));
            }
            continue;
        }

        // Extract highest priority process (min priority integer) from MinHeap
        Process* currentProc = readyHeap.extractMin();
        currentProc->state = ProcessState::RUNNING;

        int startTime = currentTime;
        // Non-preemptive execution: runs until remaining time reaches 0
        currentTime += currentProc->burstTime;
        currentProc->remainingTime = 0;
        currentProc->completionTime = currentTime;
        currentProc->calculateMetrics();
        currentProc->state = ProcessState::COMPLETED;

        completedCount++;
        executionHistory.push(ExecutionStep(currentProc->id, startTime, currentTime));

        // Enqueue any processes that arrived during this execution window into MinHeap
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            processes[arrivalIdx].state = ProcessState::READY;
            readyHeap.insert(&processes[arrivalIdx]);
            arrivalIdx++;
        }
    }

    totalCpuTime = currentTime;
    calculateAggregateMetrics(processManager);
}
