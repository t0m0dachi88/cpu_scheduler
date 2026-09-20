# System Architecture — DSA CPU Scheduling Simulator

## 1. Overview
The **DSA CPU Scheduling Simulator** is an educational C++ console application designed to simulate Operating System process scheduling. Its defining characteristic is the **strict use of custom data structures** (Queue, Circular Queue, Min Heap, Stack, Linked List) for algorithm logic instead of C++ STL containers.

---

## 2. Directory Structure Architecture

```text
DSA-CPU-Scheduler/
│
├── src/
│   ├── core/                  # Core domain models (Process model, Process State)
│   │   ├── Process.h
│   │   └── Process.cpp
│   │
│   ├── dsa/                   # Custom Data Structure implementations
│   │   ├── CustomQueue.h      # FIFO Queue for FCFS
│   │   ├── CircularQueue.h    # Circular Queue for Round Robin
│   │   ├── MinHeap.h          # Priority Queue for Priority Scheduling
│   │   └── Stack.h            # Stack for Execution History
│   │
│   ├── schedulers/            # Scheduling Algorithms
│   │   ├── SchedulerBase.h    # Base abstract class interface
│   │   ├── FCFS.h             # First-Come First-Served
│   │   ├── RoundRobin.h       # Round Robin (Time Quantum)
│   │   └── PriorityScheduler.h# Non-preemptive Priority Scheduler
│   │
│   ├── utils/                 # Utilities and Analyzers
│   │   ├── SearchSort.h       # Custom Searching & Sorting algorithms
│   │   ├── PerformanceAnalyzer.h # Metrics aggregator & comparer
│   │   └── Visualizer.h       # ASCII Gantt chart renderer
│   │
│   └── main.cpp               # CLI Entry point
│
├── data/                      # Input process workload files
│   └── processes.txt
│
├── output/                    # Exported execution reports
│   └── reports/
│
├── docs/                      # Documentation
│   ├── implementation/        # Sequential task documentation
│   ├── architecture/          # Architectural design notes
│   └── testing/               # Test suites and results
│
├── Makefile                   # Build configuration
└── README.md                  # Project overview & usage guide
```

---

## 3. High-Level Data Flow

```text
                    USER / INPUT FILE
                            |
                            v
                    PROCESS WORKLOAD
                            |
                            v
                    ARRIVAL CHECK LOOP
                            |
           +----------------+----------------+
           |                |                |
           v                v                v
         FCFS          Round Robin        Priority
       (Custom Queue) (Circular Queue)   (Min Heap)
           |                |                |
           +----------------+----------------+
                            |
                            v
                     CPU EXECUTION
                            |
                            v
                    EXECUTION HISTORY
                     (Custom Stack)
                            |
                            v
                   METRIC COMPUTATION
                 (CT, TAT = CT-AT, WT = TAT-BT)
                            |
                            v
                   PERFORMANCE ANALYZER
                 & ASCII GANTT VISUALIZER
```

---

## 4. Key Educational Principles
1. **Custom Data Structures**: Core queues, heaps, stacks, and search/sort operations are manually implemented without `std::queue`, `std::priority_queue`, `std::stack`, `std::vector`, or `std::sort`.
2. **Modular Design**: Complete decoupling between data structures, process models, schedulers, and output visualizers.
3. **Traceability**: Every time slice execution can be pushed to an execution history stack to allow step-by-step audit of CPU scheduling decisions.
