# Task: Round Robin (RR) Scheduler Implementation

## Status

Completed

## Objective

Implement the preemptive **Round Robin (RR)** CPU Scheduling algorithm (`src/schedulers/RoundRobin.h` and `src/schedulers/RoundRobin.cpp`) integrating `CircularQueue<Process*>`, configurable Time Quantum (default `Q = 2`), remaining burst time tracking, process preemption and rotation, dynamic arrivals during quantum slices, metric computations (\(CT\), \(TAT\), \(WT\)), and execution history stack logging (`Stack<ExecutionStep>`).

## What Was Implemented

1. `RoundRobin` Scheduler Class:
   - Inherits from `SchedulerBase`.
   - Configurable `timeQuantum` attribute with getter `getTimeQuantum()` and setter `setTimeQuantum()`.
   - `getAlgorithmName()`: Returns `"Round Robin (RR, Quantum = X)"`.
   - `runSimulation(ProcessManager& processManager)`:
     - Chronologically sorts processes by Arrival Time using `SearchSort::sortByArrivalTime()`.
     - Initializes `CircularQueue<Process*>` ready queue.
     - Simulates time-sliced CPU execution: `execTime = min(timeQuantum, currentProc->remainingTime)`.
     - Advances clock time and decrements `remainingTime`.
     - Enqueues newly arrived processes before re-enqueuing preempted processes to ensure correct OS queueing priority.
     - Rotates/re-enqueues incomplete processes (`remainingTime > 0`) back to the circular queue.
     - Logs execution steps to `executionHistory` stack.
     - Computes overall average metrics (`avgWaitingTime`, `avgTurnaroundTime`, `totalCpuTime`, `idleTime`).
2. Dedicated Unit Test Suite (`tests/test_round_robin.cpp`):
   - 3 test cases verifying Section 24 benchmark metrics (Q=2), CPU idle gaps, and quantum slice behavior (Q=1).
3. Main Demonstration (`src/main.cpp`):
   - Updated `main.cpp` to execute and display both FCFS and Round Robin simulation results on the benchmark workload.
4. Makefile Integration:
   - Updated `Makefile` to compile `src/schedulers/RoundRobin.cpp` and build `test_round_robin.exe` in `build/tests_bin/`.
5. Test Report:
   - Created `docs/testing/test_round_robin_results.md`.

## How It Works

`RoundRobin::runSimulation` maintains a `CircularQueue<Process*>`. In each iteration, the front process is dequeued and given up to `timeQuantum` units of CPU execution time. If its `remainingTime` drops to 0, it completes and its metrics are calculated. If `remainingTime > 0`, it is re-enqueued to the rear of the `CircularQueue`. New processes arriving during the time slice are enqueued prior to the re-enqueueing of the preempted process, maintaining strict arrival-order queueing integrity.

## DSA / CS Concepts Used

- **Circular Queue Ring Buffer**: Managing dynamic process rotation during quantum-sliced scheduling.
- **Preemptive CPU Time-Slicing**: Context switching processes after quantum expiration (`timeQuantum`).
- **Remaining Burst Time Tracking**: Decoupling original `burstTime` from dynamic `remainingTime`.
- **Execution History Logging**: Auditing time slices using `Stack<ExecutionStep>`.

## Files Created

- `src/schedulers/RoundRobin.h`
- `src/schedulers/RoundRobin.cpp`
- `tests/test_round_robin.cpp`
- `docs/testing/test_round_robin_results.md`
- `docs/implementation/012-round-robin-scheduler.md`

## Files Modified

- `src/main.cpp`
- `Makefile`

## Important Classes / Functions

- `class RoundRobin`: Round Robin scheduler class.
- `RoundRobin::runSimulation()`: Preemptive time-slicing simulation loop.
- `RoundRobin::setTimeQuantum()`: Configures time quantum length.

## Design Decisions

- **Circular Queue for Process Rotation**: Used custom `CircularQueue<Process*>` without STL containers, utilizing array wrap-around indexing for efficient rotation.
- **Queueing Order Priority**: Enqueued newly arrived processes before re-enqueuing preempted processes to mirror standard OS Round Robin scheduling specifications.

## Testing

1. Executed `make test`.
2. Verified all 3 `RoundRobin` tests passed (`3 / 3 PASSED`).
3. Executed `./cpu_scheduler.exe` and verified Section 24 benchmark metrics (Total CPU Time = 16 units, Avg TAT = 11.33, Avg WT = 6.00).

## Test Cases

| Test Case | Workload (AT, BT, Q) | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | P1(0,5), P2(1,3), P3(2,8) Q=2 | CT: 12, 9, 16; Avg TAT: 11.33; Avg WT: 6.00 | 11.33, 6.00 | PASS |
| TC-002 | P1(0,3), P2(6,2) Q=2 | Total CPU Time=8, Idle Time=3 | Total=8, Idle=3 | PASS |
| TC-003 | P1(0,4), P2(0,4) Q=1 | Alternating execution; CT: 7, 8 | CT: 7, 8 | PASS |

## Known Limitations

- Priority Scheduling (Non-preemptive using `MinHeap`) will be implemented in Phase 7.

## Next Step

**Phase 6 Complete!**
Proceed to **Phase 7: Priority Scheduling Implementation** — starting with **Task 7.1: Non-preemptive Priority Scheduler** (`src/schedulers/PriorityScheduler.h` and `src/schedulers/PriorityScheduler.cpp` integrating `MinHeap<Process*>`).

## Codebase Notes for Future Agents

- Run `make test` anytime changes are made to `RoundRobin` or `CircularQueue`.
- Time Quantum defaults to `2` but can be adjusted via `setTimeQuantum(q)`.
