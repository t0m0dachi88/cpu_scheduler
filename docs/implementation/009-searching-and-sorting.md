# Task: Custom Searching and Sorting Implementation

## Status

Completed

## Objective

Implement custom searching algorithms (Linear Search, Binary Search) and sorting algorithms (Insertion Sort, Merge Sort) in `src/utils/SearchSort.h` and `src/utils/SearchSort.cpp` built strictly from scratch without C++ STL sorting routines (`std::sort`, `std::binary_search`), supporting process sorting by **Arrival Time**, **Burst Time**, **Priority**, and **PID**.

## What Was Implemented

1. `SearchSort` Utility Class:
   - `insertionSort()`: In-place stable Insertion Sort in \(\mathcal{O}(n^2)\) time.
   - `mergeSort()`: Divide-and-conquer stable Merge Sort in \(\mathcal{O}(n \log n)\) time.
   - `sortByArrivalTime()`: Sorts processes by Arrival Time (tie-breaker PID).
   - `sortByBurstTime()`: Sorts processes by Burst Time (tie-breaker Arrival Time).
   - `sortByPriority()`: Sorts processes by Priority (1 highest priority, tie-breaker Arrival Time).
   - `sortByPID()`: Sorts processes by PID.
   - `linearSearchByPID()`: Sequential PID search in \(\mathcal{O}(n)\) time.
   - `binarySearchByPID()`: Binary PID search in \(\mathcal{O}(\log n)\) time; checks `isSortedByPID()` and falls back to linear search if array is unsorted.
2. Dedicated Unit Test Suite (`tests/test_search_sort.cpp`):
   - 6 test cases verifying sorting by arrival, burst, priority, equivalence between insertion and merge sort, linear search, and binary search.
3. Makefile Integration:
   - Updated `Makefile` to compile `src/utils/SearchSort.cpp` and added `test_search_sort.exe` target to `make test`.
4. Test Report:
   - Created `docs/testing/test_search_sort_results.md`.

## How It Works

Sorting key comparisons are centralized in `compareProcesses()`. `mergeSort()` recursively splits array range `[left, right]` at `mid`, sorts halves, and merges them using dynamic temporary arrays `L` and `R`. `binarySearchByPID()` inspects `mid = low + (high - low) / 2` and narrows bounds `[low, high]` logarithmically on PID-sorted process arrays.

## DSA / CS Concepts Used

- **Divide-and-Conquer Sorting (Merge Sort)**: \(\mathcal{O}(n \log n)\) recursive divide, sort, and merge algorithm.
- **In-Place Insertion Sort**: \(\mathcal{O}(n^2)\) comparison sort ideal for small or nearly-sorted datasets.
- **Sequential vs Logarithmic Search**: Comparing \(\mathcal{O}(n)\) Linear Search with \(\mathcal{O}(\log n)\) Binary Search.
- **Stability & Tie-Breaking Rules**: Preserving secondary ordering (Arrival Time / PID) during tie resolution.

## Files Created

- `src/utils/SearchSort.h`
- `src/utils/SearchSort.cpp`
- `tests/test_search_sort.cpp`
- `docs/testing/test_search_sort_results.md`
- `docs/implementation/009-searching-and-sorting.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `class SearchSort`: Utility class for searching and sorting.
- `SearchSort::mergeSort()`: \(\mathcal{O}(n \log n)\) stable sort.
- `SearchSort::insertionSort()`: \(\mathcal{O}(n^2)\) in-place sort.
- `SearchSort::binarySearchByPID()`: Logarithmic PID search.

## Design Decisions

- **Deterministic Tie-Breaking**: Enforced explicit tie-breaking criteria in `compareProcesses()` (e.g., if priorities match, earlier arrival time comes first) to prevent non-deterministic sorting order during scheduler process queueing.
- **Binary Search Precondition Check**: Implemented `isSortedByPID()` inside `binarySearchByPID()` to prevent silent incorrect results when binary search is invoked on unsorted arrays.

## Testing

1. Executed `make test`.
2. Verified all 6 `SearchSort` tests passed (`6 / 6 PASSED`).
3. Verified overall pass rate (`38 / 38 PASSED` across 6 test suites).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Sort AT (3, 0, 2, 1) | Sorted AT: 0, 1, 2, 3 | 0, 1, 2, 3 | PASS |
| TC-002 | Sort BT (8, 2, 5, 1) | Sorted BT: 1, 2, 5, 8 | 1, 2, 5, 8 | PASS |
| TC-003 | Sort Priority (3, 1@AT1, 2, 1@AT0) | Prio 1@AT0, Prio 1@AT1, Prio 2, Prio 3 | P4, P2, P3, P1 | PASS |
| TC-004 | Insertion vs Merge Sort | Identical outputs | Identical outputs | PASS |
| TC-005 | Linear Search PID 25 | Returns index 1; -1 for non-existent | 1, -1 | PASS |
| TC-006 | Binary Search PID 20 | Returns index 2; -1 for non-existent | 2, -1 | PASS |

## Known Limitations

- Phase 3 searching and sorting is fully complete. Scheduler abstract base class will be created in Phase 4.

## Next Step

**Phase 3 Complete!**
Proceed to **Phase 4: Scheduler Framework** — starting with **Task 4.1: Abstract Base Class `SchedulerBase`** (`src/schedulers/SchedulerBase.h`).

## Codebase Notes for Future Agents

- Use `SearchSort::sortByArrivalTime()` before initializing ready queues in FCFS or Priority Schedulers.
- Never use `std::sort` anywhere in the codebase.
