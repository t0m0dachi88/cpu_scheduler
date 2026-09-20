#include "schedulers/PriorityScheduler.h"
#include "utils/SearchSort.h"
#include <algorithm> // for std::max

bool prioritySchedulerComparator(Process* const& a, Process* const& b) {
    if (a->priority != b->priority) {
        return a->priority < b->priority; // Lower integer = higher priority
    }
    if (a->arrivalTime != b->arrivalTime) {
        return a->arrivalTime < b->arrivalTime; // Earlier arrival first
    }
    return a->id < b->id; // Lower PID first
}

PriorityScheduler::PriorityScheduler(int aging) 
    : agingInterval(aging > 0 ? aging : 5) {}

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
            stateTracker.transitionProcess(processes[arrivalIdx], ProcessState::READY, currentTime);
            readyHeap.insert(&processes[arrivalIdx]);
            arrivalIdx++;
        }

        // Apply aging to all processes currently in the ready queue
        if (readyHeap.getCount() > 0) {
            Process** heapBuffer = readyHeap.getBuffer();
            bool priorityChanged = false;
            for (int i = 0; i < readyHeap.getCount(); ++i) {
                Process* p = heapBuffer[i];
                int waitTime = currentTime - p->arrivalTime;
                if (waitTime > 0) {
                    int newPriority = std::max(1, p->originalPriority - (waitTime / agingInterval));
                    if (newPriority != p->priority) {
                        p->priority = newPriority;
                        priorityChanged = true;
                    }
                }
            }
            // If any priorities were modified, rebuild the heap to restore MinHeap property
            if (priorityChanged) {
                readyHeap.rebuild();
            }
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
        stateTracker.transitionProcess(*currentProc, ProcessState::RUNNING, currentTime);

        int startTime = currentTime;
        // Non-preemptive execution: runs until remaining time reaches 0
        currentTime += currentProc->burstTime;
        currentProc->remainingTime = 0;
        currentProc->completionTime = currentTime;
        currentProc->calculateMetrics();
        stateTracker.transitionProcess(*currentProc, ProcessState::COMPLETED, currentTime);

        completedCount++;
        executionHistory.push(ExecutionStep(currentProc->id, startTime, currentTime));

        // Enqueue any processes that arrived during this execution window into MinHeap
        while (arrivalIdx < count && processes[arrivalIdx].arrivalTime <= currentTime) {
            stateTracker.transitionProcess(processes[arrivalIdx], ProcessState::READY, currentTime);
            readyHeap.insert(&processes[arrivalIdx]);
            arrivalIdx++;
        }
    }

    totalCpuTime = currentTime;
    calculateAggregateMetrics(processManager);
}
