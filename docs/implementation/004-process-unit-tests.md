# Task: Process Unit Tests

## Status

Completed

## Objective

Create a dedicated unit test suite (`tests/test_process.cpp`), configure Makefile test targets (`make test`), test all capabilities and edge cases of `Process` and `ProcessManager`, and record test outcomes in `docs/testing/test_process_results.md`.

## What Was Implemented

1. `tests/test_process.cpp`:
   - Custom lightweight macro-based test framework (`ASSERT_TRUE`, `ASSERT_EQUAL`).
   - 8 comprehensive test cases covering:
     - Default field initialization (`testProcessDefaultConstructor`)
     - Parameterized constructor clamping (`testProcessParameterizedConstructorClamping`)
     - Metric formulas \(TAT = CT - AT\) and \(WT = TAT - BT\) (`testProcessMetricCalculation`)
     - State reset logic (`testProcessReset`)
     - Enum string formatting (`testProcessStateToString`)
     - Validation rule enforcement (`testProcessManagerValidation`)
     - Dynamic raw pointer array resizing (`testProcessManagerDynamicResizing`)
     - Workload clearing and resetting (`testProcessManagerClearAndReset`)
2. `Makefile` Updates:
   - Added `TEST_TARGET = test_process.exe` rule and `test` target (`make test`).
3. `Process` Enhancement:
   - Added `std::ostream& operator<<(std::ostream& os, ProcessState state)` in `src/core/Process.h` & `src/core/Process.cpp` for seamless stream printing.
4. Test Results Documentation:
   - Logged full test execution details in `docs/testing/test_process_results.md`.

## How It Works

`make test` compiles `tests/test_process.cpp`, `src/core/Process.cpp`, and `src/core/ProcessManager.cpp` into `test_process.exe` and executes it. Each test function exercises specific functions or boundary cases, returning `true` on success. The main runner aggregates pass/fail counts and exits with `0` if all tests pass or `1` if any test fails.

## DSA / CS Concepts Used

- **Automated Unit Testing & Test-Driven Verification**: Independent verification of domain entity contracts.
- **Dynamic Buffer Resizing Verification**: Asserting data integrity and memory allocation during dynamic array expansion.
- **Boundary & Edge Case Testing**: Testing negative inputs, zero values, duplicate keys, and zero-capacity initializations.

## Files Created

- `tests/test_process.cpp`
- `docs/testing/test_process_results.md`
- `docs/implementation/004-process-unit-tests.md`

## Files Modified

- `src/core/Process.h`
- `src/core/Process.cpp`
- `Makefile`

## Important Classes / Functions

- `tests/test_process.cpp`: Main test suite entry point.
- `std::ostream& operator<<(std::ostream& os, ProcessState state)`: Stream formatting operator for process states.

## Design Decisions

- **Lightweight Test Harness**: Avoided external testing frameworks (GTest/Catch2) to maintain zero external library dependencies and adhere to the project's educational focus.
- **Automated Makefile Integration**: Made `make test` a first-class build target to allow immediate execution of tests at any stage.

## Testing

1. Executed `make test`.
2. All 8 tests passed with 0 failures (`8 / 8 tests passed`).

## Test Cases

| Test ID | Test Name | Assertion | Result | Status |
|---|---|---|---|---|
| UT-PROC-001 | Default Constructor | Defaults match 0/NEW | 0 / NEW | PASS |
| UT-PROC-002 | Constructor Clamping | Negative inputs clamp to 1/0 | Clamped correctly | PASS |
| UT-PROC-003 | Metric Calculation | TAT=8, WT=2 | TAT=8, WT=2 | PASS |
| UT-PROC-004 | Process Reset | Remaining=BT, CT=0 | Reset correctly | PASS |
| UT-PROC-005 | State String | Enum maps to string | Mapped correctly | PASS |
| UT-PM-001 | Input Validation | Invalid cases return false | Rejected correctly | PASS |
| UT-PM-002 | Dynamic Resizing | Count=5 after 2-capacity init | Resized to 5 | PASS |
| UT-PM-003 | Reset & Clear | State=NEW, Count=0 | Cleared correctly | PASS |

## Known Limitations

- Tests cover Phase 1 (Process Management). Data structure test suites for Queue, Circular Queue, Heap, Stack will be added in Phase 2.

## Next Step

**Phase 1 Complete!**
Proceed to **Phase 2: Custom Data Structures** — starting with **Task 2.1: Custom Queue Implementation** (`src/dsa/CustomQueue.h` and `src/dsa/CustomQueue.cpp` for FCFS scheduling).

## Codebase Notes for Future Agents

- Run `make test` anytime changes are made to `Process` or `ProcessManager`.
