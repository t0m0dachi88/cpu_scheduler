# Task: Project Skeleton & Directory Structure Setup

## Status

Completed

## Objective

Set up the project file and directory skeleton for the **DSA CPU Scheduling Simulator**, configure the `Makefile` build system using `g++`, create the main entry point (`src/main.cpp`), and establish the high-level architecture documentation (`docs/architecture.md`).

## What Was Implemented

1. Directory hierarchy:
   - `src/core/` — Domain entity models (`Process`).
   - `src/dsa/` — Custom data structures (`Queue`, `CircularQueue`, `MinHeap`, `Stack`).
   - `src/schedulers/` — Scheduler implementations (`SchedulerBase`, `FCFS`, `RoundRobin`, `PriorityScheduler`).
   - `src/utils/` — Utility modules (`SearchSort`, `PerformanceAnalyzer`, `Visualizer`).
   - `data/` — Data files for workload inputs.
   - `output/reports/` — Simulation export directory.
   - `docs/implementation/` — Sequential task documentation.
   - `docs/architecture/` — Architectural documentation.
   - `docs/testing/` — Test logs and test suites.
   - `build/` — Directory for compiled object files.
2. Entry Point (`src/main.cpp`): Clean entry point displaying an initial system welcome header.
3. Build System (`Makefile`): Modular Makefile using C++17 flags (`-std=c++17 -Wall -Wextra -Isrc`) targeting `cpu_scheduler.exe`.
4. Architecture Doc (`docs/architecture.md`): Architectural blueprint outlining directory structure, data flow, and key educational constraints.

## How It Works

The build system leverages GNU `make` and `g++`. Source files under `src/` are compiled into separate object files under `build/` before linking into the binary executable `cpu_scheduler.exe`. The directory structure separates domain entities, custom data structures, schedulers, and output visualization into dedicated modules.

## DSA / CS Concepts Used

- **Modular System Architecture**: Decoupling components into discrete directories based on responsibility (Core, DSA, Schedulers, Utils).
- **Build Automation & Compilation Pipeline**: Compiling source files into object files (`.o`) and linking them into a runnable executable using `Makefile`.

## Files Created

- `src/main.cpp`
- `Makefile`
- `docs/architecture.md`
- `docs/implementation/001-project-skeleton.md`

## Files Modified

- None (initial setup)

## Important Classes / Functions

- `main()` in `src/main.cpp`: Entry point for the simulation program.

## Design Decisions

- **C++ Standard**: Used `-std=c++17` for standard C++ features while strictly avoiding STL containers in custom DSA modules.
- **Include Path**: Configured `-Isrc` in `Makefile` so headers can be included cleanly without deep relative paths (e.g., `#include "core/Process.h"`).
- **Cross-platform Makefile**: Formatted clean `make` and `clean` rules compatible with GCC on Windows (MSYS2/MinGW).

## Testing

1. Compilation Verification: Executed `make` to compile `src/main.cpp` into `build/main.o` and link to `cpu_scheduler.exe`.
2. Binary Execution Verification: Executed `./cpu_scheduler.exe` and checked output text and exit status code `0`.

## Test Cases

| Test Case | Command | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC-001: Build System | `make` | Compiles clean with 0 warnings/errors | Build succeeded cleanly | PASS |
| TC-002: Execution | `./cpu_scheduler.exe` | Prints header and returns exit code 0 | Header printed successfully | PASS |

## Known Limitations

- No process modeling or data structures implemented yet (planned for subsequent tasks).

## Next Step

**Task 1.2**: Implement `Process` model (`src/core/Process.h` and `src/core/Process.cpp`) with all required process attributes (PID, Arrival Time, Burst Time, Priority, Remaining Time, Completion Time, Turnaround Time, Waiting Time, Process State).

## Codebase Notes for Future Agents

- All new custom data structure files MUST be placed in `src/dsa/`.
- Never import `<queue>`, `<stack>`, `<vector>`, or `<algorithm>` inside core algorithm code in `src/dsa/` or `src/schedulers/`.
- Ensure headers use standard include guards or `#pragma once`.
