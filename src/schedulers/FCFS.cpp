#include "schedulers/FCFS.h"
#include "utils/SearchSort.h"

void FCFS::runSimulation(ProcessManager& processManager) {
    reset();

    int count = processManager.getCount();
    if (count == 0) return;

    // Enforce chronological sorting by Arrival Time
    SearchSort::sortByArrivalTime(processManager.getProcessesMutable(), count);
    processManager.resetAll();

    Process* processes = processManager.getProcessesMutable();
    CustomQueue<Process*> readyQueue;

    int currentTime = 0;
    int completedCount = 0;
    int arrivalIdx = 0;

    while (completedCount < count) {
        // Enqueue all processes that have arrived by currentTime
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            processes[arrivalIdx].state = ProcessState::READY;
            readyQueue.enqueue(&processes[arrivalIdx]);
            arrivalIdx++;
        }

        // If readyQueue is empty but processes remain, CPU is idle
        if (readyQueue.isEmpty()) {
            if (arrivalIdx < count) {
                int idleStart = currentTime;
                currentTime = processes[arrivalIdx].arrivalTime;
                idleTime += (currentTime - idleStart);
                executionHistory.push(ExecutionStep(-1, idleStart, currentTime));
            }
            continue;
        }

        // Dequeue process at front of FIFO queue
        Process* currentProc = readyQueue.dequeue();
        currentProc->state = ProcessState::RUNNING;

        int startTime = currentTime;
        currentTime += currentProc->burstTime;
        currentProc->remainingTime = 0;
        currentProc->completionTime = currentTime;
        currentProc->calculateMetrics();
        currentProc->state = ProcessState::COMPLETED;

        completedCount++;
        executionHistory.push(ExecutionStep(currentProc->id, startTime, currentTime));

        // Enqueue processes that arrived during this execution window
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            processes[arrivalIdx].state = ProcessState::READY;
            readyQueue.enqueue(&processes[arrivalIdx]);
            arrivalIdx++;
        }
    }

    totalCpuTime = currentTime;
    calculateAggregateMetrics(processManager);
}
