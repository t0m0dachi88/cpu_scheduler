# Task: Performance Analyzer — Comparative Algorithm Benchmarking

## Status

Completed

## Objective

Implement `src/utils/PerformanceAnalyzer.h` and `src/utils/PerformanceAnalyzer.cpp` — a comparative analysis engine that runs all three scheduling algorithms (FCFS, Round Robin, Priority) on the same process workload and renders a side-by-side performance comparison table with best-value highlighting.

## What Was Implemented

1. `AlgorithmResult` Struct (`src/utils/PerformanceAnalyzer.h`):
   - Holds per-algorithm metrics: `algorithmName`, `totalCpuTime`, `idleTime`, `avgTurnaround`, `avgWaiting`, `cpuUtilization`.
   - `cpuUtilization` is computed as `(totalCpuTime - idleTime) / totalCpuTime * 100.0`.

2. `PerformanceAnalyzer` Class (`src/utils/PerformanceAnalyzer.h` / `.cpp`):
   - `analyze(ProcessManager&)`:
     - Creates a local FCFS, RoundRobin(quantum), and PriorityScheduler instance.
     - Runs each on the same workload, calling `processManager.resetAll()` between runs.
     - Records metrics into `results[MAX_ALGORITHMS]` fixed-size array — no STL containers.
   - `printComparisonTable()`:
     - Scans results for best Avg TAT (lowest), best Avg WT (lowest), best CPU Util (highest).
     - Renders a formatted ASCII table with `*` markers on best values per column.
     - Prints a footer summary with best values.
   - `getResult(int index)` — const accessor with bounds checking.
   - `getResultCount()` — returns number of valid results.

3. Test Suite (`tests/test_performance_analyzer.cpp`) — 7 test cases:
   - TC-001: `analyze()` records exactly 3 results (FCFS, RR, Priority).
   - TC-002: FCFS result matches Section 24 benchmark (Avg TAT=8.67, Avg WT=3.33, CPU=16, idle=0, util=100%).
   - TC-003: RR result matches benchmark (Avg TAT=11.33, Avg WT=6.00).
   - TC-004: Priority result matches benchmark (Avg TAT=8.67, Avg WT=3.33).
   - TC-005: CPU utilization formula verified with idle gap workload (62.5%).
   - TC-006: Empty workload handled gracefully (0 results, no crash).
   - TC-007: `printComparisonTable()` renders without crash.

4. `main.cpp` Updated:
   - Added Step 6: `PerformanceAnalyzer analyzer(2)` → `analyze(manager)` → `printComparisonTable()`.

5. Makefile Integration:
   - `PerformanceAnalyzer.cpp` added to SRCS/OBJS.
   - Compile rules for `PerformanceAnalyzer.o` and `test_performance_analyzer.o`.
   - `TEST_PERFORMANCE_ANALYZER_OBJS`, link rule, and `make test` runner entry added.

## How It Works

`analyze()` instantiates each scheduler in a local scope, runs `runSimulation()` against the shared `ProcessManager`, records the metrics via `recordResult()`, then calls `processManager.resetAll()` to restore process states (CT, TAT, WT, state → NEW) for the next algorithm. This ensures all three schedulers operate on identical initial conditions.

`printComparisonTable()` iterates through results to find minimum TAT, minimum WT, and maximum utilization, then renders each row comparing against these best values with `*` markers.

## DSA / CS Concepts Used

- **Fixed-Size Array Storage**: `results[MAX_ALGORITHMS]` avoids STL `vector` — uses a fixed ceiling of 8 algorithm slots.
- **Workload Isolation via Reset**: `ProcessManager::resetAll()` resets CT/TAT/WT/state so each scheduler starts from the same initial conditions without reconstructing the process list.
- **CPU Utilization Formula**: `utilization = (totalTime - idleTime) / totalTime * 100%` — standard OS metric.
- **Best-Value Identification**: Linear scan for min/max across results — O(n) where n = number of algorithms.

## Files Created

- `src/utils/PerformanceAnalyzer.h`
- `src/utils/PerformanceAnalyzer.cpp`
- `tests/test_performance_analyzer.cpp`
- `docs/implementation/015-performance-analyzer.md`

## Files Modified

- `src/main.cpp` — Added PerformanceAnalyzer step.
- `Makefile` — Added all compile/link/test rules.

## Testing

1. `make clean; make` — **0 warnings, 0 errors**.
2. `./cpu_scheduler.exe` — Full output with Gantt charts + comparison table rendered correctly.
3. `make test` — **54 / 54 tests passed** across all 12 test suites.

## Test Cases

| Test Case | Verified | Status |
|---|---|---|
| TC-001 Analyze records 3 results | resultCount == 3 | PASS |
| TC-002 FCFS metrics (Section 24) | TAT=8.67, WT=3.33, CPU=16, idle=0, util=100% | PASS |
| TC-003 RR metrics (Section 24) | TAT=11.33, WT=6.00 | PASS |
| TC-004 Priority metrics | TAT=8.67, WT=3.33 | PASS |
| TC-005 CPU utilization with idle gap | 62.5% (5 busy / 8 total) | PASS |
| TC-006 Empty workload | 0 results, no crash | PASS |
| TC-007 printComparisonTable | Renders table, no crash | PASS |

## Sample Comparison Table Output

```
  ==========================================================================
                    ALGORITHM PERFORMANCE COMPARISON
  ==========================================================================
  (* = best value in that column)

  +---------------------------------+----------+----------+----------+----------+----------+
  | Algorithm                       |  CPU Time|   Avg TAT|    Avg WT| Idle Time| CPU Util|
  +---------------------------------+----------+----------+----------+----------+----------+
  | First-Come First-Served (FCFS)  |        16|     8.67*|     3.33*|      0   |  100.00*|
  | Round Robin (RR, Quantum = 2)   |        16|    11.33 |     6.00 |      0   |  100.00*|
  | Priority Scheduling (Non-pre... |        16|     8.67*|     3.33*|      0   |  100.00*|
  +---------------------------------+----------+----------+----------+----------+----------+

  Summary:
    Best Avg Turnaround Time : 8.67 units
    Best Avg Waiting Time    : 3.33 units
    Best CPU Utilization     : 100.00 %
```

## Known Limitations

- Algorithm name column is truncated to 31 chars — very long names get `...` suffix.
- Currently hardcodes all 3 schedulers; future phases may allow user to select which algorithms to compare.

## Codebase Notes for Future Agents

- `PerformanceAnalyzer::analyze()` calls `processManager.resetAll()` between runs — this is essential for fair comparison.
- `AlgorithmResult` is a plain struct with a default constructor — safe to store in fixed-size arrays.
- `MAX_ALGORITHMS = 8` can be increased if more scheduling algorithms are added in future phases.
- The `rrQuantum` parameter defaults to 2 and is configurable via the constructor.
- `getResult(index)` throws `std::out_of_range` for invalid indices.
