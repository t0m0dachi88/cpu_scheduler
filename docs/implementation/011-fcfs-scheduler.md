# Task: First-Come First-Served (FCFS) Scheduler Implementation

## Status

Completed

## Objective

Implement the non-preemptive **First-Come First-Served (FCFS)** CPU Scheduling algorithm (`src/schedulers/FCFS.h` and `src/schedulers/FCFS.cpp`) integrating `CustomQueue<Process*>`, chronological arrival pre-sorting (`SearchSort::sortByArrivalTime`), dynamic process arrival handling, CPU idle time tracking, metric computations (\(CT\), \(TAT\), \(WT\)), and execution history stack logging (`Stack<ExecutionStep>`).

## What Was Implemented

1. `FCFS` Scheduler Class:
   - Inherits from `SchedulerBase`.
   - `getAlgorithmName()`: Returns `"First-Come First-Served (FCFS)"`.
   - `runSimulation(ProcessManager& processManager)`:
     - Sorts workload by Arrival Time using `SearchSort::sortByArrivalTime()`.
     - Initializes custom FIFO ready queue `CustomQueue<Process*>`.
     - Simulates clock ticks (`currentTime`), enqueuing processes as they arrive.
     - Handles CPU idle gaps when no processes are ready.
     - Updates process metrics (\(CT\), \(TAT\), \(WT\)) and lifecycle states (`NEW` -> `READY` -> `RUNNING` -> `COMPLETED`).
     - Logs execution steps to `executionHistory` stack.
     - Computes overall average metrics (`avgWaitingTime`, `avgTurnaroundTime`, `totalCpuTime`, `idleTime`).
2. Dedicated Unit Test Suite (`tests/test_fcfs.cpp`):
   - 3 test cases verifying benchmark workload results, CPU idle gap tracking, and arrival time pre-sorting.
3. Main Demonstration (`src/main.cpp`):
   - Updated `main.cpp` to run FCFS simulation on the Section 24 benchmark workload.
4. Makefile Integration:
   - Updated `Makefile` to compile `src/schedulers/FCFS.cpp` and build `test_fcfs.exe` in `build/tests_bin/`.
5. Test Report:
   - Created `docs/testing/test_fcfs_results.md`.

## How It Works

`FCFS::runSimulation` sorts processes chronologically by arrival time. At `currentTime = 0`, arrived processes are enqueued into `CustomQueue<Process*>`. The scheduler dequeues the process at the front of the FIFO queue, sets state to `RUNNING`, advances `currentTime` by `burstTime`, marks completion time `completionTime = currentTime`, computes metrics, sets state to `COMPLETED`, and logs execution to `executionHistory`. Any processes arriving during execution are enqueued before selecting the next process.

## DSA / CS Concepts Used

- **First-In, First-Out Queue (FIFO)**: Queue-based non-preemptive CPU scheduling strategy.
- **Process State Lifecycle**: Transitioning process states through `NEW`, `READY`, `RUNNING`, and `COMPLETED`.
- **CPU Idle Time Tracking**: Detecting gaps where `readyQueue` is empty and advancing clock time to the next arrival.
- **Execution Audit Logging**: Storing time-slice steps in `Stack<ExecutionStep>`.

## Files Created

- `src/schedulers/FCFS.h`
- `src/schedulers/FCFS.cpp`
- `tests/test_fcfs.cpp`
- `docs/testing/test_fcfs_results.md`
- `docs/implementation/011-fcfs-scheduler.md`

## Files Modified

- `src/main.cpp`
- `Makefile`

## Important Classes / Functions

- `class FCFS`: First-Come First-Served scheduler class.
- `FCFS::runSimulation()`: Non-preemptive FIFO CPU simulation loop.

## Design Decisions

- **Custom FIFO Queue**: Integrated `CustomQueue<Process*>` without using `std::queue` or `std::vector`, strictly adhering to the project's educational DSA guidelines.
- **Pointers in Ready Queue**: Enqueued pointers (`Process*`) to update the central process object state directly without copying objects.

## Testing

1. Executed `make test`.
2. Verified all 3 `FCFS` tests passed (`3 / 3 PASSED`).
3. Executed `./cpu_scheduler.exe` and verified Section 24 benchmark metrics (Avg TAT = 8.67, Avg WT = 3.33).

## Test Cases

| Test Case | Workload (AT, BT) | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | P1(0,5), P2(1,3), P3(2,8) | CT: 5, 8, 16; Avg TAT: 8.67; Avg WT: 3.33 | 8.67, 3.33 | PASS |
| TC-002 | P1(0,3), P2(5,2) | Total CPU Time=7, Idle Time=2 | Total=7, Idle=2 | PASS |
| TC-003 | P2(2,4), P1(0,3) (Out of order) | P1 AT0 runs first (CT 3), then P2 (CT 7) | P1 first, CT=3,7 | PASS |

## Known Limitations

- FCFS is non-preemptive and cannot rotate processes across time quanta (Round Robin will be implemented in Phase 6).

## Next Step

**Phase 5 Complete!**
Proceed to **Phase 6: Round Robin Scheduler Implementation** — starting with **Task 6.1: Round Robin Scheduler** (`src/schedulers/RoundRobin.h` and `src/schedulers/RoundRobin.cpp` integrating `CircularQueue<Process*>` and configurable Time Quantum).

## Codebase Notes for Future Agents

- Run `make test` to verify FCFS scheduling calculations anytime process model changes occur.
- FCFS does not preempt processes once execution starts.
