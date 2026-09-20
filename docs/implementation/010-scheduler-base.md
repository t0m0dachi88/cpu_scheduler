# Task: Scheduler Framework Base Class

## Status

Completed

## Objective

Implement the abstract base class `SchedulerBase` (`src/schedulers/SchedulerBase.h`) to establish a uniform interface for all scheduling algorithms (FCFS, Round Robin, Priority), enforce chronological process arrival sorting (`SearchSort::sortByArrivalTime()`), aggregate average performance metrics (WT, TAT, total CPU time, idle time), and maintain CPU time-slice execution history using `Stack<ExecutionStep>`.

## What Was Implemented

1. `ExecutionStep` Struct:
   - Stores PID (`-1` for CPU idle), `startTime`, and `endTime` for each CPU time slice.
2. `SchedulerBase` Abstract Base Class:
   - Pure virtual functions: `runSimulation(ProcessManager&)` and `getAlgorithmName()`.
   - Virtual reset function: `reset()` clearing metrics and `executionHistory`.
   - Performance metrics calculation: `calculateAggregateMetrics()` computing average Turnaround Time (\(TAT\)) and average Waiting Time (\(WT\)).
   - Summary reporter: `printSummary()` outputting formatted metric reports.
   - Execution history stack: `Stack<ExecutionStep> executionHistory`.
3. Dedicated Unit Test Suite (`tests/test_scheduler_base.cpp`):
   - 3 test cases verifying polymorphic dispatch, metric aggregation, and arrival sorting.
4. Makefile Integration:
   - Added `test_scheduler_base.exe` target to `make test`.
5. Test Report:
   - Created `docs/testing/test_scheduler_base_results.md`.

## How It Works

Derived scheduler classes (e.g. `FCFS`, `RoundRobin`, `PriorityScheduler`) inherit from `SchedulerBase` and implement `runSimulation(ProcessManager&)`. Before scheduling begins, the workload is chronologically sorted by arrival time (`SearchSort::sortByArrivalTime`). As the simulation executes, time slices are pushed to `executionHistory`. Upon completion, `calculateAggregateMetrics()` computes overall average TAT and WT.

## DSA / CS Concepts Used

- **Object-Oriented Abstract Interfaces (Polymorphism)**: Enforcing contract compliance across algorithm strategies (`SchedulerBase*`).
- **Standardized Metric Computation**: Centralizing aggregate Turnaround Time (\(\text{Avg TAT} = \sum TAT / n\)) and Waiting Time (\(\text{Avg WT} = \sum WT / n\)).
- **Execution History Auditing**: Recording time slices using `Stack<ExecutionStep>`.
- **Chronological Workload Sorting**: Pre-sorting processes by arrival time before populating ready queues.

## Files Created

- `src/schedulers/SchedulerBase.h`
- `tests/test_scheduler_base.cpp`
- `docs/testing/test_scheduler_base_results.md`
- `docs/implementation/010-scheduler-base.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `struct ExecutionStep`: Execution step metadata.
- `class SchedulerBase`: Abstract base class for all schedulers.
- `SchedulerBase::calculateAggregateMetrics()`: Aggregates process metrics.
- `SchedulerBase::printSummary()`: Displays simulation summary table.

## Design Decisions

- **Polymorphic Base Pointer**: Derived schedulers can be passed as `SchedulerBase*` pointers to the performance analyzer, allowing uniform algorithm comparison.
- **Enforced Arrival Pre-sorting**: Integrated `SearchSort::sortByArrivalTime()` into the base simulation pipeline to prevent scheduling processes out of chronological arrival order.

## Testing

1. Executed `make test`.
2. Verified all 3 `SchedulerBase` tests passed (`3 / 3 PASSED`).
3. Verified total test suite pass rate (`41 / 41 PASSED` across 7 test runners).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Polymorphic call | `getAlgorithmName()="Mock Scheduler"` | `"Mock Scheduler"` | PASS |
| TC-002 | Metric aggregation (P1: AT0, BT5; P2: AT1, BT3) | Total Time=8, Avg TAT=6.0, Avg WT=2.0 | 8, 6.0, 2.0 | PASS |
| TC-003 | Out-of-order arrivals (P2 AT5, P1 AT0) | Sorted: P1 AT0 first, idle 1 unit | P1 first, idle=1 | PASS |

## Known Limitations

- FCFS Scheduler implementation will be integrated in Phase 5 Task 5.1.

## Next Step

**Phase 4 Complete!**
Proceed to **Phase 5: FCFS Scheduler Implementation** — starting with **Task 5.1: FCFS Scheduler** (`src/schedulers/FCFS.h` and `src/schedulers/FCFS.cpp` using `CustomQueue<Process*>`).

## Codebase Notes for Future Agents

- Inherit from `SchedulerBase` when creating `FCFS`, `RoundRobin`, or `PriorityScheduler`.
- Always invoke `calculateAggregateMetrics(processManager)` at the end of `runSimulation()`.
