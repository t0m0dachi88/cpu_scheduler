# Task: Custom Min Heap Implementation

## Status

Completed

## Objective

Implement a template-based `MinHeap` data structure (`src/dsa/MinHeap.h`) using an array-based binary min heap with custom comparator support built strictly from scratch without C++ STL containers (`std::priority_queue`, `std::make_heap`), designed for **Non-preemptive Priority Scheduling**.

## What Was Implemented

1. `MinHeap<T>` Class:
   - Dynamic array buffer (`heap`), `capacity`, `count`, and function pointer comparator (`compareFunc`).
   - 0-based array index formulas (`parent(i) = (i-1)/2`, `leftChild(i) = 2i+1`, `rightChild(i) = 2i+2`).
   - `insert(const T& value)`: Appends element to end and performs `heapifyUp()` in \(\mathcal{O}(\log n)\) time. Automatically resizes buffer when full.
   - `extractMin()`: Replaces root with last element, performs `heapifyDown()` in \(\mathcal{O}(\log n)\) time, and returns minimum element; throws `std::underflow_error` on empty heap.
   - `peekMin()`: Peeks root element in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty heap.
   - `isEmpty()`, `size()`, `getCapacity()`, `clear()`.
   - Custom comparator support allowing specialized sorting strategies (such as `Process*` priority comparator: lower priority integer = higher CPU priority).
2. Dedicated Unit Test Suite (`tests/test_min_heap.cpp`):
   - 6 test cases verifying heap ordering, duplicate priorities, dynamic array resizing, underflow exceptions, and `Process*` priority comparator.
3. Makefile Integration:
   - Added `test_min_heap.exe` target to `make test`.
4. Test Report:
   - Created `docs/testing/test_min_heap_results.md`.

## How It Works

`MinHeap<T>` stores binary heap nodes in contiguous array memory `heap`. When inserting an element, `heapifyUp()` compares the new element with its parent using `compareFunc(heap[index], heap[parent])` and swaps upwards until the min heap property is restored. When extracting the minimum element, `extractMin()` saves `heap[0]`, moves `heap[count-1]` to root position, and `heapifyDown()` swaps downwards with the smallest child until heap ordering is restored.

## DSA / CS Concepts Used

- **Binary Min Heap Data Structure**: Complete binary tree implemented in dynamic array memory.
- **Heapify Operations**: Upward (`heapifyUp`) and downward (`heapifyDown`) logarithmic tree restructuring.
- **Function Pointer Comparators**: Customizable ordering criteria supporting complex object priority comparisons.
- **Dynamic Memory Allocation**: Array expansion without relying on STL `std::vector`.

## Files Created

- `src/dsa/MinHeap.h`
- `tests/test_min_heap.cpp`
- `docs/testing/test_min_heap_results.md`
- `docs/implementation/007-custom-min-heap.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `class MinHeap<T>`: Array-based binary min heap.
- `MinHeap::insert()`: Logarithmic insertion.
- `MinHeap::extractMin()`: Logarithmic minimum extraction.
- `MinHeap::peekMin()`: Constant-time root inspection.

## Design Decisions

- **Function Pointer Comparator**: Passed comparator `bool (*comp)(const T&, const T&)` in the constructor so `MinHeap` can be reused for integers, process priorities, or custom aging criteria without code duplication.
- **Array-Based Binary Tree**: Used 0-based array index navigation for optimal cache locality and \(\mathcal{O}(1)\) parent/child index calculations.

## Testing

1. Executed `make test`.
2. Verified all 6 `MinHeap` tests passed (`6 / 6 PASSED`).
3. Verified total test suite pass rate (`26 / 26 PASSED` across process, queue, circular queue, and min heap test runners).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Initial state (cap=5) | `isEmpty()=true`, `size()=0`, `getCapacity()=5` | `true`, `0`, `5` | PASS |
| TC-002 | Insert 50,20,40,10,30 | `extractMin()` returns 10,20,30,40,50 | 10, 20, 30, 40, 50 | PASS |
| TC-003 | Insert duplicates (5,2,5,2,1) | `extractMin()` returns 1,2,2,5,5 | 1, 2, 2, 5, 5 | PASS |
| TC-004 | Empty `extractMin()` / `peekMin()` | Throws `std::underflow_error` | Threw underflow_error | PASS |
| TC-005 | Insert beyond capacity (2 -> 3) | Capacity doubles to 4 | Capacity = 4 | PASS |
| TC-006 | Insert P1(Prio 2), P2(Prio 1), P3(Prio 3), P4(Prio 1, AT 0) | Extracted order: P4, P2, P1, P3 | P4, P2, P1, P3 | PASS |

## Known Limitations

- Stack data structure for Execution History will be implemented in Task 2.7.

## Next Step

**Phase 2 Task 2.7**: Implement `Stack` (`src/dsa/Stack.h`) — custom LIFO Stack data structure for tracking **CPU Execution History**.

## Codebase Notes for Future Agents

- `MinHeap` will serve as the ready queue for **Non-preemptive Priority Scheduler** (Phase 7).
- Pass `processPriorityComparator` when constructing `MinHeap<Process*>` for Priority Scheduling.
