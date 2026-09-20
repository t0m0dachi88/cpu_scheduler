# Task: ASCII Gantt Chart Visualizer

## Status

Completed

## Objective

Implement `src/utils/Visualizer.h` and `src/utils/Visualizer.cpp` — an ASCII Gantt Chart and timeline renderer that reads the `Stack<ExecutionStep>` execution history from any `SchedulerBase` subclass and renders a proportional horizontal Gantt bar, time tick row, execution timeline legend, performance metrics summary, and per-process result table.

## What Was Implemented

1. `Visualizer` Class (`src/utils/Visualizer.h` / `src/utils/Visualizer.cpp`):
   - `static void renderGanttChart(const SchedulerBase&, const ProcessManager&, const std::string& title = "")`:
     - Collects `ExecutionStep`s from the scheduler's LIFO `Stack<ExecutionStep>` using `Stack::collectToArray()`.
     - Reverses the LIFO array in-place to restore chronological (forward) order — no STL used.
     - Renders a proportional ASCII Gantt bar: `+----+` border, centred process label (`P1`, `P2`, `IDLE`), time tick row.
     - Prints an Execution Timeline legend with per-slice `t=start → t=end (N units)` annotations.
     - Displays Performance Metrics: Total CPU Time, Idle Time, Avg TAT, Avg WT.
   - `static void renderProcessTable(const ProcessManager&)`:
     - Renders a compact per-process summary table with AT, BT, Priority, CT, TAT, WT columns.
   - `static int collectSteps(const Stack<ExecutionStep>&, ExecutionStep*, int)` *(private)*:
     - Non-destructively drains the Stack using `collectToArray()`, reverses for chronological order.
   - `static void renderBlock(const std::string& label, int duration)` *(private)*:
     - Renders a single centred label inside a proportional cell block.
   - Constants: `MAX_STEPS = 256`, `CELL_WIDTH = 4` (characters per time unit).

2. `Stack<T>::collectToArray()` added to `src/dsa/Stack.h`:
   - Non-destructive O(n) traversal of the linked-list stack.
   - Fills an external array top→bottom (LIFO order) without modifying the stack.
   - Used by Visualizer to safely read execution history without popping.

3. Dedicated Unit Test Suite (`tests/test_visualizer.cpp`) — 5 test cases:
   - TC-001: FCFS render (no crash, 3 steps verified).
   - TC-002: Idle gap handling (P1 + IDLE block + P2 chart).
   - TC-003: Chronological ordering verification (collectToArray LIFO → reversed forward).
   - TC-004: Round Robin multi-quantum step rendering (4 slices).
   - TC-005: Empty history graceful fallback (no crash, informational message).

4. Main Demonstration (`src/main.cpp`):
   - Updated: calls `Visualizer::renderGanttChart()` and `renderProcessTable()` after each of the three schedulers (FCFS, Round Robin, Priority).

5. Makefile Integration:
   - Added `src/utils/Visualizer.cpp` to SRCS/OBJS for main build.
   - Added `$(BUILD_DIR)/utils/Visualizer.o` compile rule.
   - Added `TEST_VISUALIZER_OBJS`, `$(BUILD_DIR)/tests/test_visualizer.o` compile rule, link rule, and runner to `make test`.

## How It Works

`renderGanttChart` calls `collectSteps()` → `Stack::collectToArray()` traverses the internal linked-list top→bottom (LIFO), writing to a temp array. The array is then reversed in-place to produce forward chronological order (earliest slice first). For each step, a proportional cell of `duration × CELL_WIDTH` characters is drawn. The time tick row prints start times left-aligned at each cell boundary, and the final end time at the far right.

## DSA / CS Concepts Used

- **Stack Traversal (Non-Destructive)**: `collectToArray()` walks the linked-list from `topNode` without pop, preserving the original stack.
- **In-Place Array Reversal**: Two-pointer reversal converts LIFO order to chronological order — O(n) time, O(1) auxiliary space.
- **Proportional ASCII Rendering**: Cell width ∝ execution duration — longer bursts produce wider Gantt blocks for visual accuracy.
- **Static Utility Class Pattern**: All methods are `static`, making `Visualizer` a pure utility with no instance state.

## Files Created

- `src/utils/Visualizer.h`
- `src/utils/Visualizer.cpp`
- `tests/test_visualizer.cpp`
- `docs/implementation/014-ascii-gantt-visualizer.md`

## Files Modified

- `src/dsa/Stack.h` — Added `collectToArray()` method.
- `src/main.cpp` — Added Visualizer calls after each scheduler.
- `Makefile` — Added Visualizer.cpp compile/link rules and test target.

## Important Classes / Functions

- `class Visualizer` — Static ASCII Gantt chart renderer.
- `Visualizer::renderGanttChart()` — Reads execution history → renders Gantt chart.
- `Visualizer::renderProcessTable()` — Renders per-process metrics table.
- `Stack<T>::collectToArray()` — Non-destructive LIFO stack traversal to array.

## Design Decisions

- **CELL_WIDTH = 4**: Each time unit occupies 4 characters, giving proportional visual width to process duration. Short 1-unit slices still render legibly with a minimum cell size enforced.
- **Non-destructive Stack Read**: Using `collectToArray()` instead of `pop()` ensures the execution history stack remains intact for replay/analysis after rendering.
- **In-place Reversal without STL**: Two-pointer swap reverses LIFO→chronological without `std::reverse` or `std::vector`, maintaining the no-STL constraint.
- **IDLE block support**: `ExecutionStep.pid == -1` renders as `IDLE` label, making CPU gaps visually obvious in the Gantt chart.
- **`processManager` reserved parameter**: Kept in the signature for Phase 9 (PerformanceAnalyzer) integration, where process names/colours may be derived from the workload.

## Testing

1. Executed `make` — compiled cleanly with 0 warnings, 0 errors.
2. Executed `./cpu_scheduler.exe` — Gantt charts rendered correctly for FCFS, Round Robin (9 slices), and Priority Scheduler.
3. Executed `make test` — **47 / 47** tests passed across all 11 test suites.

## Test Cases

| Test Case | Workload | Verified | Status |
|---|---|---|---|
| TC-001 | FCFS 3 processes | 3 steps, correct render, no crash | PASS |
| TC-002 | FCFS with idle gap | P1 + IDLE + P2 rendered; idle time = 3 | PASS |
| TC-003 | FCFS 3 sequential | collectToArray LIFO order; reversed → P1,P2,P3 | PASS |
| TC-004 | RR Q=2, two 4-BT processes | 4 slices rendered correctly | PASS |
| TC-005 | No simulation run | Empty history → graceful message, no crash | PASS |

## Sample Gantt Chart Output (FCFS)

```
==========================================================
  Gantt Chart: First-Come First-Served (FCFS)
==========================================================

  +--------------------+------------+--------------------------------+
  |        P1        |    P2    |              P3              |
  +--------------------+------------+--------------------------------+
  0                    5            8                                16

  Execution Timeline:
    [P1]    t=0 -> t=5  (5 units)
    [P2]    t=5 -> t=8  (3 units)
    [P3]    t=8 -> t=16  (8 units)

  Performance Metrics:
    Total CPU Time      : 16 units
    CPU Idle Time       : 0 units
    Avg Turnaround Time : 8.67 units
    Avg Waiting Time    : 3.33 units
```

## Known Limitations

- **Terminal Width**: Very long schedules (many processes, high burst times) may wrap on narrow terminals. Phase 14 CLI integration should detect terminal width and scale `CELL_WIDTH` accordingly.

## Next Step

**Phase 8 Complete!**
Proceed to **Phase 9: Performance Analyzer** — **Task 9.1**: Implement `src/utils/PerformanceAnalyzer.h` and `src/utils/PerformanceAnalyzer.cpp` to run all three schedulers on the same workload and render a comparative algorithm performance table (Avg CT, TAT, WT side-by-side).

## Codebase Notes for Future Agents

- `CELL_WIDTH` in `Visualizer.h` controls horizontal scale — increase for wider terminals.
- `MAX_STEPS = 256` is the ceiling for execution history steps in one simulation.
- `Stack<ExecutionStep>::collectToArray()` is `const` — safe to call on any `const Stack<ExecutionStep>&`.
- IDLE blocks are identified by `ExecutionStep::pid == -1`.
