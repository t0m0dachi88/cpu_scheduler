# Test Execution Report: Searching & Sorting Algorithms

## Executive Summary
- **Test Executable**: `test_search_sort.exe`
- **Source Code under test**: `src/utils/SearchSort.h`, `src/utils/SearchSort.cpp`
- **Total Tests Executed**: 6
- **Tests Passed**: 6
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-SORT-001 | Sort by Arrival Time | `SearchSort` | Verifies sorting processes in ascending order of arrival time (with PID tie-breaker). | **PASS** |
| UT-SORT-002 | Sort by Burst Time | `SearchSort` | Verifies sorting processes in ascending order of burst time (with arrival tie-breaker). | **PASS** |
| UT-SORT-003 | Sort by Priority | `SearchSort` | Verifies sorting processes in ascending order of priority number (1 highest, with arrival tie-breaker). | **PASS** |
| UT-SORT-004 | Insertion vs Merge Sort | `SearchSort` | Verifies functional equivalence and stability between Insertion Sort and Merge Sort outputs. | **PASS** |
| UT-SEARCH-001 | Linear Search by PID | `SearchSort` | Validates \(\mathcal{O}(n)\) linear lookup returning correct index or `-1`. | **PASS** |
| UT-SEARCH-002 | Binary Search by PID | `SearchSort` | Validates \(\mathcal{O}(\log n)\) binary search on PID-sorted array with automatic fallback. | **PASS** |

---

## Algorithm Complexity Analysis

| Algorithm | Best Time | Average Time | Worst Time | Space Complexity | Stable |
|---|---|---|---|---|---|
| Insertion Sort | \(\mathcal{O}(n)\) | \(\mathcal{O}(n^2)\) | \(\mathcal{O}(n^2)\) | \(\mathcal{O}(1)\) | Yes |
| Merge Sort | \(\mathcal{O}(n \log n)\) | \(\mathcal{O}(n \log n)\) | \(\mathcal{O}(n \log n)\) | \(\mathcal{O}(n)\) | Yes |
| Linear Search | \(\mathcal{O}(1)\) | \(\mathcal{O}(n)\) | \(\mathcal{O}(n)\) | \(\mathcal{O}(1)\) | N/A |
| Binary Search | \(\mathcal{O}(1)\) | \(\mathcal{O}(\log n)\) | \(\mathcal{O}(\log n)\) | \(\mathcal{O}(1)\) | N/A |

---

## Command to Reproduce

```bash
make test
```
