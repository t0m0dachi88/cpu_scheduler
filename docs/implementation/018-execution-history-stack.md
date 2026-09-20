# Task 12.1: Execution History Stack

## Status
Completed (Implemented proactively during Phases 4 & 8)

## Objective
Log CPU time-slice executions to a `Stack` data structure and implement a history playback mechanism to support the rendering of timeline visualizations (Gantt Charts).

## What Was Implemented

1.  **`ExecutionStep` Struct & History Stack (`src/schedulers/SchedulerBase.h`)**:
    *   Created the `ExecutionStep` struct to record the `pid`, `startTime`, and `endTime` of a specific continuous execution block on the CPU. A `pid` of `-1` is used to represent CPU Idle time.
    *   Added a `Stack<ExecutionStep> executionHistory` protected member to the `SchedulerBase` class, ensuring every scheduler inherits the capability to log its execution.

2.  **Scheduler Integration (`FCFS`, `RoundRobin`, `PriorityScheduler`)**:
    *   Every scheduler pushes an `ExecutionStep` onto the stack whenever a time slice completes (e.g., when a process finishes, gets preempted in Round Robin, or when an idle gap is detected).
    *   The `Stack` acts as a Last-In-First-Out (LIFO) log, recording events chronologically as they happen, meaning the most recent event is at the top of the stack.

3.  **History Playback (`src/utils/Visualizer.cpp`)**:
    *   Since a `Stack` only provides access to the most recent element, `Visualizer::renderGanttChart` utilizes the `Stack::collectToArray()` method (implemented in `src/dsa/Stack.h`) to extract all execution steps into a dynamic array without destroying the stack.
    *   The Visualizer then applies a two-pointer reversal algorithm to flip the array from reverse-chronological (LIFO) to forward-chronological order, enabling a seamless left-to-right timeline playback for rendering the Gantt chart and step-by-step trace.

## DSA / CS Concepts Used
*   **LIFO Event Logging**: Stacks are exceptionally well-suited for fast O(1) appending of execution history records.
*   **Non-Destructive Stack Traversal**: Extracting the stack elements to a secondary array structure allows for timeline playback without emptying or modifying the primary logging structure.
*   **Two-Pointer Array Reversal**: An efficient O(N) in-place algorithm used to invert the LIFO data into chronological order for human-readable output.

## Verification
*   The execution history stack was heavily verified in `tests/test_scheduler_base.cpp` and `tests/test_visualizer.cpp`.
*   Gantt charts correctly display complex interleaved timelines (like in Round Robin preemption) perfectly, proving the stack records and plays back history flawlessly.

## Next Steps
Proceeding to Phase 13: File Management (Task 13.1).
