# Test Execution Report: FCFS Scheduler Algorithm

## Executive Summary
- **Test Executable**: `test_fcfs.exe` & `cpu_scheduler.exe`
- **Source Code under test**: `src/schedulers/FCFS.h`, `src/schedulers/FCFS.cpp`
- **Total Tests Executed**: 3
- **Tests Passed**: 3
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-FCFS-001 | Section 24 Benchmark Workload | `FCFS` | Validates completion times, turnaround times, and waiting times for standard benchmark processes (P1, P2, P3). | **PASS** |
| UT-FCFS-002 | CPU Idle Time Handling | `FCFS` | Verifies tracking of CPU idle cycles when no ready processes have arrived. | **PASS** |
| UT-FCFS-003 | Arrival Time Pre-Sorting | `FCFS` | Verifies correct chronological FIFO execution when processes are added out of order. | **PASS** |

---

## Metric Verification Breakdown (Section 24 Benchmark)

Workload:
- P1: Arrival = 0, Burst = 5
- P2: Arrival = 1, Burst = 3
- P3: Arrival = 2, Burst = 8

| Process | Arrival Time | Burst Time | Completion Time (CT) | Turnaround Time (TAT = CT - AT) | Waiting Time (WT = TAT - BT) |
|---|---|---|---|---|---|
| P1 | 0 | 5 | 5 | 5 | 0 |
| P2 | 1 | 3 | 8 | 7 | 4 |
| P3 | 2 | 8 | 16 | 14 | 6 |

- **Total Elapsed CPU Time**: 16 units
- **Total CPU Idle Time**: 0 units
- **Average Turnaround Time**: \(\frac{5 + 7 + 14}{3} = \mathbf{8.67}\) units
- **Average Waiting Time**: \(\frac{0 + 4 + 6}{3} = \mathbf{3.33}\) units

---

## Command to Reproduce

```bash
make test
```
