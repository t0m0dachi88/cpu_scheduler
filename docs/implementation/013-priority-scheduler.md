# Task: Non-preemptive Priority Scheduler Implementation

## Status

Completed

## Objective

Integrate and verify the **Non-preemptive Priority Scheduler** (`src/schedulers/PriorityScheduler.h` and `src/schedulers/PriorityScheduler.cpp`) which uses a `MinHeap<Process*>` as the ready queue. Wire the scheduler into the main build target and demonstration binary, register a dedicated unit test suite, and verify correctness against the Section 24 benchmark workload.

## What Was Implemented

1. `PriorityScheduler` Scheduler Class (pre-built, now fully integrated):
   - Inherits from `SchedulerBase`.
   - `getAlgorithmName()`: Returns `"Priority Scheduling (Non-preemptive)"`.
   - `runSimulation(ProcessManager& processManager)`:
     - Sorts processes chronologically by Arrival Time using `SearchSort::sortByArrivalTime()`.
     - Initializes `MinHeap<Process*>` ready queue with custom `prioritySchedulerComparator`.
     - Enqueues all processes that have arrived by `currentTime` into the heap.
     - On CPU idle: fast-forwards `currentTime` to the next arriving process's arrival time, logs an idle `ExecutionStep(-1, ...)` to `executionHistory`.
     - Extracts the highest-priority process via `extractMin()` (lowest priority integer = highest priority).
     - Executes non-preemptively: runs to full `burstTime` without interruption.
     - Enqueues newly arrived processes after execution window into heap.
     - Computes aggregate metrics (`avgWaitingTime`, `avgTurnaroundTime`, `totalCpuTime`, `idleTime`).
2. `prioritySchedulerComparator` function:
   - Tie-breaking rule: lower priority integer → earlier arrival time → lower PID.
3. Dedicated Unit Test Suite (`tests/test_priority_scheduler.cpp`):
   - 3 test cases covering benchmark metrics, non-preemptive MinHeap selection, and CPU idle time.
4. Main Demonstration (`src/main.cpp`):
   - Updated to execute FCFS, Round Robin (Q=2), and Priority Scheduler sequentially on the benchmark workload.
5. Makefile Integration:
   - Added `src/schedulers/PriorityScheduler.cpp` to SRCS/OBJS for main build.
   - Added `test_priority_scheduler` compile rule, object list, link rule, and runner to `make test`.

## How It Works

`PriorityScheduler::runSimulation` maintains a `MinHeap<Process*>`. At each scheduling decision point, all arrived processes are inserted into the heap. The heap's comparator enforces priority ordering (lower integer = higher priority). `extractMin()` retrieves the highest-priority process, which then runs non-preemptively to completion. Any processes arriving during that execution window are inserted into the heap for the next round. CPU idle gaps are handled by advancing `currentTime` directly to the next arrival.

## DSA / CS Concepts Used

- **Min Heap (Priority Queue)**: `MinHeap<Process*>` provides O(log n) insert and O(log n) extract-min, naturally selecting the highest-priority process at each scheduling decision point.
- **Custom Comparator Function Pointer**: `prioritySchedulerComparator` enables the generic `MinHeap<T>` to sort `Process*` by priority, arrival time, and PID without STL.
- **Non-preemptive Scheduling**: Once a process is selected, it holds the CPU until `burstTime` exhausted — no context switching mid-execution.
- **Execution History Logging**: All CPU slices (active and idle) are pushed to the `Stack<ExecutionStep>` for auditability.

## Files Created

- `docs/implementation/013-priority-scheduler.md`

## Files Modified

- `src/main.cpp` — Added Priority Scheduler demo block.
- `Makefile` — Added `PriorityScheduler.cpp` to SRCS/OBJS, added test compile rule, link rule, and test runner entry.

## Important Classes / Functions

- `class PriorityScheduler` — Non-preemptive Priority Scheduler class.
- `PriorityScheduler::runSimulation()` — MinHeap-driven scheduling simulation.
- `bool prioritySchedulerComparator(Process* const& a, Process* const& b)` — Custom MinHeap ordering comparator.

## Design Decisions

- **MinHeap for Ready Queue**: `MinHeap<Process*>` with a custom comparator directly maps the OS priority scheduling model: O(log n) selection of highest-priority process without STL `priority_queue`.
- **Non-preemptive Policy**: Running to full `burstTime` mirrors real OS non-preemptive schedulers; once selected, a process cannot be interrupted by later arrivals with higher priority.
- **Tie-breaking Hierarchy**: Priority → Arrival Time → PID ensures deterministic, reproducible scheduling for processes with equal priority integers.

## Testing

1. Executed `make` — compiled cleanly with 0 warnings and 0 errors.
2. Executed `./cpu_scheduler.exe` — verified Priority Scheduler output matches Section 24 expected metrics.
3. Executed `make test` — all 42 tests passed (42 / 42).

## Test Cases

| Test Case | Workload (AT, BT, Priority) | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | P1(0,5,2), P2(1,3,1), P3(2,8,3) | CT: 5, 8, 16; Avg TAT: 8.67; Avg WT: 3.33 | 8.67, 3.33 | PASS |
| TC-002 | P1(0,10,3), P2(2,2,1), P3(2,4,2) | P1 CT=10, P2 CT=12, P3 CT=16 (non-preemptive) | 10, 12, 16 | PASS |
| TC-003 | P1(0,3,2), P2(6,2,1) | Total CPU=8, Idle Time=3 | 8, 3 | PASS |

## Simulation Output (Section 24 Benchmark)

```
SCHEDULER RESULTS: Priority Scheduling (Non-preemptive)
+-------+----------+--------+----------+--------+--------+--------+------------+
|   PID | Arrival  | Burst  | Priority |   CT   |  TAT   |   WT   |    State   |
+-------+----------+--------+----------+--------+--------+--------+------------+
|     1 |        0 |      5 |        2 |      5 |      5 |      0 |  COMPLETED |
|     2 |        1 |      3 |        1 |      8 |      7 |      4 |  COMPLETED |
|     3 |        2 |      8 |        3 |     16 |     14 |      6 |  COMPLETED |
+-------+----------+--------+----------+--------+--------+--------+------------+

Total Elapsed CPU Time : 16 units
Total CPU Idle Time    : 0 units
Average Turnaround Time: 8.67 units
Average Waiting Time   : 3.33 units
```

## Known Limitations

- **Aging Mechanism Not Implemented**: Processes with very low priority (high integer) can starve if higher-priority processes continually arrive. Aging will be addressed in Phase 11.

## Next Step

**Phase 7 Complete!**
Proceed to **Phase 8: ASCII Gantt Chart Visualizer** — **Task 8.1**: Implement `src/utils/Visualizer.h` and `src/utils/Visualizer.cpp` that reads the `Stack<ExecutionStep>` execution history from any scheduler and renders a formatted ASCII timeline/Gantt chart to the console.

## Codebase Notes for Future Agents

- Run `make test` anytime changes are made to `PriorityScheduler` or `MinHeap`.
- Priority integer semantics: **lower integer = higher priority** (e.g., Priority 1 runs before Priority 3).
- Aging implementation (Phase 11) must dynamically decrement process priority integers while in the `READY` state inside the `MinHeap`.
