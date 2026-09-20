# DSA CPU Scheduling Simulator

A comprehensive, interactive C++ CPU Scheduling Simulator built entirely from scratch without using any standard template library (STL) containers. All data structures, including Queues, Min-Heaps, and Stacks, are implemented fundamentally to demonstrate deep computer science principles.

## Features

- **Interactive CLI Menu**: Easily load workloads, input processes manually, and compare scheduling algorithms on-the-fly.
- **Three Scheduling Algorithms**:
  - **First-Come First-Served (FCFS)**: Basic non-preemptive queue scheduling.
  - **Round Robin (RR)**: Preemptive time-slicing using a custom Circular Queue. Configurable time quantum.
  - **Priority Scheduling**: Non-preemptive execution based on priority, implemented using a custom Min-Heap.
- **Aging Mechanism**: Built into the Priority Scheduler to progressively boost the priority of waiting processes and effectively prevent starvation.
- **ASCII Gantt Chart & Visualizer**: Generates an intuitive timeline visualization (Gantt Chart) and chronological process execution trace natively in the terminal.
- **State Transition Tracker**: A dedicated logging system that records and displays every state change (`NEW -> READY -> RUNNING -> COMPLETED`) alongside timestamps.
- **Performance Analyzer**: A robust engine that simulates multiple algorithms back-to-back, isolating workloads, and generates a side-by-side comparative table highlighting the lowest waiting/turnaround times and highest CPU utilization.
- **File Management**: Dynamically loads workload files (`data/processes.txt`) skipping comments and gracefully handling invalid inputs. Saves success reports to disk.

## Custom Data Structures

As part of the strict constraints of this project, absolutely no STL containers (`std::vector`, `std::queue`, `std::stack`, `std::priority_queue`, etc.) were used.

1.  **CustomQueue (`CustomQueue.h`)**: A dynamic, resizing FIFO array used primarily for FCFS.
2.  **CircularQueue (`CircularQueue.h`)**: A fixed-size array-based ring buffer that perfectly fits Round Robin's preemptive context-switching requirements, avoiding costly `O(n)` array shifting.
3.  **MinHeap (`MinHeap.h`)**: An array-based binary tree providing `O(log N)` extraction. It includes an `O(N)` `rebuild()` mechanism leveraging Floyd's algorithm to support dynamic key updates required by the Aging Mechanism.
4.  **Stack (`Stack.h`)**: A LIFO data structure with dynamic resizing. Includes a non-destructive `collectToArray()` mechanism used in conjunction with a two-pointer array reversal to convert LIFO execution logs into chronological timeline playbacks.

## Project Structure

```
cpu_scheduler/
│
├── data/
│   └── processes.txt          # Default workload file
│
├── output/
│   └── reports/               # Output directory for analysis reports
│
├── src/
│   ├── main.cpp               # Application entry point & CLI Menu
│   ├── core/                  # Core process model & ProcessManager
│   ├── dsa/                   # Custom Data Structures (Queue, Heap, Stack)
│   ├── schedulers/            # Algorithm implementations (FCFS, RR, Priority)
│   └── utils/                 # Utilities (Visualizer, Tracker, File I/O, SearchSort)
│
├── tests/                     # Unit test suites for every component
├── docs/                      # Implementation and Architecture logs
└── Makefile                   # Build system
```

## How to Build & Run

### Prerequisites
- `g++` (C++17 or higher)
- `make`

### Building the Simulator
To compile the project and generate the executable:
```bash
make clean
make
```

### Running the Simulator
To launch the interactive CLI menu:
```bash
./cpu_scheduler.exe
```
*(On Linux/macOS, use `./cpu_scheduler` if you adapt the Makefile output name)*

### Running Unit Tests
This project includes 14 test suites with 66 comprehensive unit tests verifying data structures, algorithmic constraints, edge cases (e.g. idle CPU handling), and metrics formulas.
```bash
make test
```

## Data Input Format
When using the **Load Workload from File** option (or creating your own `data/processes.txt`), ensure each process is defined on a single line with four space-separated integers:
```
<PID> <ArrivalTime> <BurstTime> <Priority>
```
*Note: A lower priority integer indicates a higher absolute priority (e.g., 1 is the highest priority).*
Lines starting with `#` or empty lines are ignored.

Example:
```text
# P1
1 0 5 2
# P2
2 1 3 1
```

## License
MIT
