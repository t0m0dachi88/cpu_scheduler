# Task: Process Model Implementation

## Status

Completed

## Objective

Implement the core `Process` class (`src/core/Process.h` and `src/core/Process.cpp`) containing process attributes (PID, Arrival Time, Burst Time, Priority, Remaining Time, Completion Time, Turnaround Time, Waiting Time, Process State), lifecycle state conversions, metric calculation routines, and tabular display utilities.

## What Was Implemented

1. `ProcessState` enum class in `Process.h`:
   - `NEW`
   - `READY`
   - `RUNNING`
   - `COMPLETED`
2. `processStateToString(ProcessState state)` helper function to map state enums to printable string names.
3. `Process` class:
   - Attributes: `id`, `arrivalTime`, `burstTime`, `priority`, `remainingTime`, `completionTime`, `turnaroundTime`, `waitingTime`, `state`.
   - Default constructor initializing default values (`PID=0`, `AT=0`, `BT=0`, `Priority=1`, `RemainingTime=0`, `State=NEW`).
   - Parameterized constructor `Process(id, arrivalTime, burstTime, priority)`.
   - `calculateMetrics()` method implementing \(TAT = CT - AT\) and \(WT = TAT - BT\).
   - `reset()` method for re-initializing process state between scheduling algorithm test runs.
   - `printProcessDetails()` method formatted for clear ASCII tabular output.
4. Updated `Makefile` to include `src/core/Process.cpp` in compilation targets.
5. Verification test in `src/main.cpp`.

## How It Works

When a process object is instantiated with `(pid, arrivalTime, burstTime, priority)`, its `remainingTime` is automatically set equal to `burstTime`, and initial metrics (`CT`, `TAT`, `WT`) are zeroed with state set to `ProcessState::NEW`. During CPU execution simulation, the scheduler updates `completionTime` upon process finish and invokes `calculateMetrics()`, which automatically updates `turnaroundTime` (\(CT - AT\)) and `waitingTime` (\(TAT - BT\)).

## DSA / CS Concepts Used

- **Operating System Process Control Block (PCB) Representation**: Abstracting process metadata and execution state within a unified C++ class structure.
- **Process State Lifecycle**: Modeling state transitions (`NEW`, `READY`, `RUNNING`, `COMPLETED`).
- **Scheduling Metrics**: Turnaround Time (\(TAT = CT - AT\)) and Waiting Time (\(WT = TAT - BT\)).

## Files Created

- `src/core/Process.h`
- `src/core/Process.cpp`
- `docs/implementation/002-process-model.md`

## Files Modified

- `src/main.cpp`
- `Makefile`

## Important Classes / Functions

- `class Process`: Primary process model.
- `Process::calculateMetrics()`: Computes `turnaroundTime` and `waitingTime`.
- `Process::reset()`: Resets dynamic scheduling metrics for re-simulations.
- `processStateToString(ProcessState)`: Converts `ProcessState` enum to string.

## Design Decisions

- **Priority Convention**: Lower integer values represent higher priority (`1` is highest), matching standard OS scheduling benchmarks.
- **Automatic Initialization**: `remainingTime` is set to `burstTime` in the constructor to prepare processes for preemptive or quantum-based scheduling (Round Robin).

## Testing

1. Test Execution: Created sample processes (`P1`, `P2`, `P3`) in `src/main.cpp`.
2. Verified manual completion times (`CT=5`, `CT=8`, `CT=16`).
3. Checked calculated output table for correct `TAT` and `WT` values.

## Test Cases

| Test Case | Inputs (AT, BT, CT) | Expected TAT | Expected WT | Actual Result | Status |
|---|---|---|---|---|---|
| P1 Metrics | AT=0, BT=5, CT=5 | TAT=5 | WT=0 | TAT=5, WT=0 | PASS |
| P2 Metrics | AT=1, BT=3, CT=8 | TAT=7 | WT=4 | TAT=7, WT=4 | PASS |
| P3 Metrics | AT=2, BT=8, CT=16 | TAT=14 | WT=6 | TAT=14, WT=6 | PASS |

## Known Limitations

- Processes are manually created in code; interactive CLI input and file parsing will be added in Phase 1 Task 1.3 and Phase 13.

## Next Step

**Task 1.3**: Implement Process Input & Dynamic Process Creation interface in `src/main.cpp` or helper utility to allow creating processes interactively.

## Codebase Notes for Future Agents

- Any scheduler calculating completion time MUST call `process.calculateMetrics()` before reporting turnaround or waiting times.
- Priority `1` is higher priority than priority `2`.
