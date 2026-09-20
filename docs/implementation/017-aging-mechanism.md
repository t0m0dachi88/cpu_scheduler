# Task 11.1: Aging Mechanism

## Status
Completed

## Objective
Implement an Aging Mechanism in the non-preemptive Priority Scheduler to progressively increase the priority of long-waiting processes in the ready queue, preventing starvation of low-priority tasks by a steady stream of higher-priority arrivals.

## What Was Implemented

1.  **Process Model Extension (`src/core/Process.h` & `.cpp`)**:
    *   Added `int originalPriority` to track the initial priority assigned to the process.
    *   Updated the constructors to initialize both `priority` and `originalPriority`.
    *   Updated the `reset()` method to restore the dynamic `priority` back to `originalPriority` before each simulation run.

2.  **MinHeap Upgrade (`src/dsa/MinHeap.h`)**:
    *   Added `getBuffer()` and `getCount()` to allow external iteration over the heap's underlying array buffer.
    *   Added `rebuild()` method which re-heapifies the entire buffer in O(N) time. This is critical for restoring the MinHeap property after modifying the keys (priorities) of elements already inside the heap.

3.  **Aging Logic in Priority Scheduler (`src/schedulers/PriorityScheduler.h` & `.cpp`)**:
    *   Added an `agingInterval` configurable parameter (default = 5 time units) via the constructor.
    *   Updated `getAlgorithmName()` to display the current aging configuration.
    *   In the `runSimulation()` loop, before checking if the `readyHeap` is empty, added logic to:
        1. Iterate over all processes currently in the `readyHeap`.
        2. Calculate their current waiting time (`currentTime - arrivalTime`).
        3. If `waitTime > 0`, calculate a new priority: `max(1, originalPriority - (waitTime / agingInterval))`.
        4. If any priority was decreased (higher priority), flag it and call `readyHeap.rebuild()` to restore the heap property.

4.  **Testing (`tests/test_priority_scheduler.cpp`)**:
    *   Added a new robust test case, `testAgingMechanism()`, that sets up a starvation scenario.
    *   Verifies that a low-priority process (`P2`), which would normally be starved by late-arriving medium-priority processes (`P4`), eventually has its priority aged up to outcompete them.
    *   Asserts the correct execution timeline with aging (P1 -> P3 -> **P2** -> P4) vs without aging (P1 -> P3 -> **P4** -> P2).

## How It Works
The `PriorityScheduler` evaluates the waiting time of all ready processes at every scheduling point (i.e., when a running process finishes and the CPU becomes available). For every complete `agingInterval` (e.g., 5 time units) a process has waited, its priority integer is decreased by 1 (which represents an *increase* in scheduling priority). The priority is clamped at a minimum value of 1. If any priorities change, the `MinHeap` is completely rebuilt in O(N) time.

## DSA / CS Concepts Used
*   **Aging for Starvation Prevention**: A classic OS scheduling technique to ensure bounded waiting times for all processes.
*   **O(N) Heap Rebuild (Floyd's Algorithm)**: Modifying keys inside a heap breaks the heap property. Extracting all elements and re-inserting them would take O(N log N). Instead, we modify the elements in place and call `rebuild()` which uses a bottom-up `heapifyDown` approach to restore the heap in optimal O(N) time.

## Verification
*   `make clean; make` executes with 0 warnings/errors.
*   `make test` confirms 62/62 tests pass across 13 suites, including the specific starvation/aging verification.

## Next Steps
Proceeding to Phase 12: Execution History Stack (Task 12.1).
