#include "schedulers/RoundRobin.h"
#include "utils/SearchSort.h"
#include <algorithm>

RoundRobin::RoundRobin(int quantum)
    : timeQuantum(quantum > 0 ? quantum : 2) {}

void RoundRobin::runSimulation(ProcessManager& processManager) {
    reset();

    int count = processManager.getCount();
    if (count == 0) return;

    // Enforce chronological sorting by Arrival Time
    SearchSort::sortByArrivalTime(processManager.getProcessesMutable(), count);
    processManager.resetAll();

    Process* processes = processManager.getProcessesMutable();
    CircularQueue<Process*> readyQueue(count > 10 ? count : 10);

    int currentTime = 0;
    int completedCount = 0;
    int arrivalIdx = 0;

    while (completedCount < count) {
        // Enqueue all processes that have arrived by currentTime
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            stateTracker.transitionProcess(processes[arrivalIdx], ProcessState::READY, currentTime);
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

        // Dequeue process at front of circular queue
        Process* currentProc = readyQueue.dequeue();
        stateTracker.transitionProcess(*currentProc, ProcessState::RUNNING, currentTime);

        int startTime = currentTime;
        int execTime = std::min(timeQuantum, currentProc->remainingTime);

        currentTime += execTime;
        currentProc->remainingTime -= execTime;

        executionHistory.push(ExecutionStep(currentProc->id, startTime, currentTime));

        // Enqueue any new processes that arrived during this time slice BEFORE re-enqueueing currentProc
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            stateTracker.transitionProcess(processes[arrivalIdx], ProcessState::READY, currentTime);
            readyQueue.enqueue(&processes[arrivalIdx]);
            arrivalIdx++;
        }

        // Check if current process has finished or needs to be re-enqueued to back of queue
        if (currentProc->remainingTime == 0) {
            currentProc->completionTime = currentTime;
            currentProc->calculateMetrics();
            stateTracker.transitionProcess(*currentProc, ProcessState::COMPLETED, currentTime);
            completedCount++;
        } else {
            stateTracker.transitionProcess(*currentProc, ProcessState::READY, currentTime);
            readyQueue.enqueue(currentProc);
        }
    }

    totalCpuTime = currentTime;
    calculateAggregateMetrics(processManager);
}
