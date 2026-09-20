# Task: Process Input & Dynamic Process Creation

## Status

Completed

## Objective

Implement a `ProcessManager` class (`src/core/ProcessManager.h` and `src/core/ProcessManager.cpp`) to handle process collection storage using raw dynamic memory arrays, robust input validation (PID uniqueness, positive burst times, non-negative arrival times, positive priorities), interactive console input helpers, and workload management utilities.

## What Was Implemented

1. `ProcessManager` Class:
   - Dynamic memory array storage (`Process* processes`) with automatic resizing (`resize()`).
   - Deep memory management (`new[]` in constructor/resize, `delete[]` in destructor).
   - `validateProcessInput()` checking:
     - `PID > 0` (positive integer).
     - `PID` uniqueness (`existsPID()` check).
     - `Arrival Time >= 0` (non-negative).
     - `Burst Time > 0` (positive).
     - `Priority > 0` (positive integer).
   - `addProcess()` overload accepting parameters or `Process` instances.
   - `inputProcessFromConsole()` interactive prompt with robust `cin` input stream error handling.
   - `displayAll()` tabular output of loaded workload.
   - `resetAll()` to reset dynamic metrics across simulation runs.
2. Makefile updates:
   - Resolved `[HIGH]` issue: Fixed `Makefile` target rules to automatically create `$(BUILD_DIR)` and `$(BUILD_DIR)/core` directories before running `g++`, ensuring clean builds (`make clean && make`) work reliably.
3. Process constructor clamping:
   - Resolved `[LOW]` issue: Updated `Process(pid, arrival, burst, priority)` constructor to clamp values (`pid >= 1`, `arrival >= 0`, `burst >= 1`, `priority >= 1`).

## How It Works

`ProcessManager` manages a dynamically resizing raw C-style array (`Process* processes`) rather than relying on STL containers (`std::vector`). When adding a process, `validateProcessInput()` checks all mathematical and structural constraints. If capacity is exhausted, `resize()` doubles the capacity, copies existing processes to the new buffer, and frees the old buffer using `delete[]`.

## DSA / CS Concepts Used

- **Dynamic Memory Allocation**: Raw C++ memory management (`new[]`, `delete[]`) for custom dynamic arrays without STL `std::vector`.
- **Input Validation & Data Integrity**: Guarding against negative burst times, negative arrival times, and duplicate identifiers.
- **Resource Management (RAII)**: Clean constructor allocation and destructor deallocation avoiding memory leaks.

## Files Created

- `src/core/ProcessManager.h`
- `src/core/ProcessManager.cpp`
- `docs/implementation/003-process-input.md`

## Files Modified

- `src/core/Process.cpp`
- `src/main.cpp`
- `Makefile`

## Important Classes / Functions

- `class ProcessManager`: Primary workload container and validator.
- `ProcessManager::validateProcessInput()`: Central input validation logic.
- `ProcessManager::existsPID()`: Linear lookup verifying PID uniqueness.
- `ProcessManager::resize()`: Memory buffer expansion.

## Design Decisions

- **Custom Dynamic Array**: Implemented raw pointer dynamic array allocation in `ProcessManager` to uphold the STL policy of avoiding `std::vector` for core workload data structures.
- **Strict Validation Rules**: Rejected invalid input combinations (such as duplicate PIDs or zero burst times) explicitly with error messages rather than silently producing corrupt simulation metrics.

## Testing

1. Tested `make clean && make` to verify that `build/` directory is automatically created.
2. Verified error output for duplicate PIDs (`PID=1`).
3. Verified error output for negative arrival times (`AT=-2`).
4. Verified error output for non-positive burst times (`BT=0`).
5. Verified error output for non-positive priorities (`Priority=0`).
6. Verified loading standard Section 24 benchmark processes (`P1`, `P2`, `P3`).

## Test Cases

| Test Case | Input Parameters | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC-001: Clean Build | `make clean && make` | Compiles clean without missing directory error | Build succeeded cleanly | PASS |
| TC-002: Duplicate PID | PID=1 (already loaded) | Reject with error message | Rejected with duplicate PID error | PASS |
| TC-003: Negative Arrival | AT=-2 | Reject with error message | Rejected with negative arrival error | PASS |
| TC-004: Zero Burst Time | BT=0 | Reject with error message | Rejected with zero burst error | PASS |
| TC-005: Zero Priority | Priority=0 | Reject with error message | Rejected with zero priority error | PASS |
| TC-006: Valid Workload | P1(0,5,2), P2(1,3,1), P3(2,8,3) | Added cleanly, total count=3 | Loaded 3 processes | PASS |

## Known Limitations

- Processes are currently added via code calls or basic console input; file I/O loading from `data/processes.txt` will be added in Phase 13.

## Next Step

**Phase 1 Task 1.4**: Implement unit tests for Process and ProcessManager models, wrapping up Phase 1 (Process Management) before proceeding to Phase 2 (Custom Data Structures).

## Codebase Notes for Future Agents

- Always use `ProcessManager` for storing and validating workloads.
- Do NOT substitute `Process*` arrays with `std::vector` inside `ProcessManager`.
