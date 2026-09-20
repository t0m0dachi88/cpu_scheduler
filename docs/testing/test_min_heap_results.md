# Test Execution Report: MinHeap Data Structure

## Executive Summary
- **Test Executable**: `test_min_heap.exe`
- **Source Code under test**: `src/dsa/MinHeap.h`
- **Total Tests Executed**: 6
- **Tests Passed**: 6
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-HEAP-001 | Initial State | `MinHeap` | Verifies initial empty state, count=0, default capacity setup. | **PASS** |
| UT-HEAP-002 | Integer Ordering | `MinHeap` | Validates `heapifyUp()` and `heapifyDown()` sorting elements in min-heap order. | **PASS** |
| UT-HEAP-003 | Duplicate Priorities | `MinHeap` | Verifies correct ordering and extraction when duplicate values are inserted. | **PASS** |
| UT-HEAP-004 | Underflow Exceptions | `MinHeap` | Validates `std::underflow_error` on empty `extractMin()` and empty `peekMin()`. | **PASS** |
| UT-HEAP-005 | Dynamic Buffer Resizing | `MinHeap` | Validates dynamic array doubling (`resize()`) when inserting elements beyond initial capacity. | **PASS** |
| UT-HEAP-006 | Priority Comparator | `MinHeap` | Verifies `MinHeap<Process*>` using priority comparator (lower prio number = higher priority). | **PASS** |

---

## Time & Space Complexity

| Operation | Time Complexity | Space Complexity |
|---|---|---|
| `insert(item)` | \(\mathcal{O}(\log n)\) | \(\mathcal{O}(1)\) |
| `extractMin()` | \(\mathcal{O}(\log n)\) | \(\mathcal{O}(1)\) |
| `peekMin()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `isEmpty()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `size()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `resize()` | \(\mathcal{O}(n)\) | \(\mathcal{O}(n)\) |

---

## Command to Reproduce

```bash
make test
```
