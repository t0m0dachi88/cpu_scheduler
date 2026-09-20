# Test Execution Report: Process & ProcessManager Models

## Executive Summary
- **Test Executable**: `test_process.exe`
- **Source Code under test**: `src/core/Process.h`, `src/core/Process.cpp`, `src/core/ProcessManager.h`, `src/core/ProcessManager.cpp`
- **Total Tests Executed**: 8
- **Tests Passed**: 8
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-PROC-001 | Process Default Constructor | `Process` | Verifies default field initialization (`PID=0`, `AT=0`, `BT=0`, `State=NEW`). | **PASS** |
| UT-PROC-002 | Constructor Clamping | `Process` | Verifies clamping of negative/zero initialization values to valid lower bounds. | **PASS** |
| UT-PROC-003 | Metric Calculation | `Process` | Validates formulas \(TAT = CT - AT\) and \(WT = TAT - BT\). | **PASS** |
| UT-PROC-004 | Process Reset | `Process` | Verifies resetting dynamic metrics (`remainingTime`, `CT`, `TAT`, `WT`, `state`). | **PASS** |
| UT-PROC-005 | State String Conversion | `ProcessState` | Validates `processStateToString()` and `operator<<` formatting. | **PASS** |
| UT-PM-001 | Input Validation Rules | `ProcessManager` | Validates error rejection for duplicate PIDs, negative arrival, zero burst, zero priority. | **PASS** |
| UT-PM-002 | Dynamic Memory Resizing | `ProcessManager` | Tests dynamic array memory expansion when adding more elements than initial capacity. | **PASS** |
| UT-PM-003 | Workload Reset & Clear | `ProcessManager` | Tests resetting state across loaded workload and clearing elements. | **PASS** |

---

## Command to Reproduce

```bash
make test
```
