# Task 13.1: File Management

## Status
Completed

## Objective
Implement File I/O operations to enable the CPU scheduler to load process workloads from a text file (`data/processes.txt`) and save summary reports to an output directory (`output/reports/`).

## What Was Implemented

1.  **`FileManager` Class (`src/utils/FileManager.h` & `.cpp`)**:
    *   `loadProcessesFromFile()`: Reads a text file line-by-line. Safely ignores empty lines and comments (lines starting with `#`). Parses strictly formatted lines (`PID ArrivalTime BurstTime Priority`) into the `ProcessManager`.
    *   `saveReportToFile()`: Writes a raw string containing formatted simulation outputs (or performance reports) to a designated text file.

2.  **Default Data File**:
    *   Created `data/processes.txt` containing the Section 24 benchmark workload with comments demonstrating the format.

3.  **Main Integration (`src/main.cpp`)**:
    *   Replaced the hardcoded benchmark processes with a call to `FileManager::loadProcessesFromFile("data/processes.txt", manager)`.
    *   Added a fallback mechanism: If the file is missing or empty, it gracefully loads the default benchmark processes.
    *   Added a call at the end of the simulation to write a success summary to `output/reports/performance_report.txt` using `saveReportToFile()`.

4.  **Testing (`tests/test_file_manager.cpp`)**:
    *   Created 4 test cases verifying:
        1. Valid file loading (ignoring comments/blanks).
        2. Graceful rejection and skipping of malformed lines.
        3. Saving a report and reading it back to verify contents.
        4. Safe handling of non-existent files.
    *   Integrated the suite into `Makefile` and `make test`.

## How It Works
The `FileManager` isolates all `std::ifstream` and `std::ofstream` logic away from the core simulation. During loading, `std::stringstream` is used to efficiently parse the integers out of each line. If a line cannot successfully parse 4 integers, it prints a warning to `std::cerr` and skips the line without crashing the program, ensuring robust data ingestion.

## DSA / CS Concepts Used
*   **Decoupled I/O (Separation of Concerns)**: Moving File I/O out of `ProcessManager` and into a dedicated `FileManager` utility class keeps the core simulation logic clean and strictly focused on data structures and algorithms.
*   **Robust Stream Parsing**: Utilizing string streams to validate format before attempting to populate core data models prevents corruption of the simulation state.

## Verification
*   `make clean; make` executes with 0 warnings/errors.
*   `make test` confirms 66/66 tests pass across 14 test suites.
*   `./cpu_scheduler.exe` successfully loads `data/processes.txt`, executes all algorithms, and emits the success report to `output/reports/performance_report.txt`.

## Next Steps
Proceeding to the final phase: Phase 14: Final CLI Menu & Integration (Task 14.1).
