# Task: Custom Stack Implementation

## Status

Completed

## Objective

Implement a template-based `Stack` data structure (`src/dsa/Stack.h`) using dynamic singly linked nodes built strictly from scratch without C++ STL containers (`std::stack`, `std::vector`), designed for tracking **CPU Execution History** in LIFO order.

## What Was Implemented

1. `StackNode<T>` struct:
   - Contains data of generic type `T` and pointer to next node (`StackNode* next`).
2. `Stack<T>` Class:
   - Generic LIFO Stack built with singly linked nodes (`topNode`, `count`).
   - `push(const T& value)`: Pushes element onto top of stack in \(\mathcal{O}(1)\) time.
   - `pop()`: Removes and returns top element in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty stack.
   - `top()`: Peeks top element without removal in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty stack.
   - `isEmpty()`: Returns whether count is 0 in \(\mathcal{O}(1)\) time.
   - `size()`: Returns current element count in \(\mathcal{O}(1)\) time.
   - `clear()`: Iteratively pops all nodes in \(\mathcal{O}(n)\) time.
   - `printStack()`: Debug helper printing stack contents from top to bottom.
   - Destructor: Automatic RAII node deallocation via `clear()`.
3. Dedicated Unit Test Suite (`tests/test_stack.cpp`):
   - 6 test cases verifying LIFO order, exception throwing, execution history tracking (`Process*`), empty state, and node deallocation.
4. Makefile Integration:
   - Added `test_stack.exe` target to `make test`.
5. Test Report:
   - Created `docs/testing/test_stack_results.md`.

## How It Works

`Stack<T>` maintains a pointer to `topNode`. Pushing creates a new `StackNode<T>` on the heap, sets `newNode->next = topNode`, and points `topNode` to `newNode`. Popping saves `topNode->data`, updates `topNode = topNode->next`, deallocates the unlinked top node using `delete`, and decrements `count`.

## DSA / CS Concepts Used

- **Stack Data Structure (LIFO)**: Last-In, First-Out dynamic data structure.
- **CPU Execution History Tracking**: Recording time-slice process execution steps to allow auditing CPU decisions.
- **Singly Linked Dynamic Allocation**: Heap node allocation providing strict \(\mathcal{O}(1)\) push and pop without array resizing overhead.
- **Generic Programming & RAII**: C++ template support and automatic memory deallocation.

## Files Created

- `src/dsa/Stack.h`
- `tests/test_stack.cpp`
- `docs/testing/test_stack_results.md`
- `docs/implementation/008-custom-stack.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `struct StackNode<T>`: Node representation.
- `class Stack<T>`: Custom LIFO stack class.
- `Stack::push()`: Constant-time push to top.
- `Stack::pop()`: Constant-time pop from top.
- `Stack::top()`: Constant-time top inspection.

## Design Decisions

- **Linked List Stack**: Selected pointer-linked nodes for `Stack` to guarantee strict \(\mathcal{O}(1)\) operations for every `push` without periodic array re-allocation latency spikes.

## Testing

1. Executed `make test`.
2. Verified all 6 `Stack` tests passed (`6 / 6 PASSED`).
3. Verified total suite pass rate (`32 / 32 PASSED` across all 5 test runners).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Initial state | `isEmpty()=true`, `size()=0` | `true`, `0` | PASS |
| TC-002 | Push 10, 20, 30 & Pop 3x | 30, then 20, then 10 | 30, 20, 10 | PASS |
| TC-003 | Push 100 & `top()` | `top()=100`, `size()=1` | `100`, `1` | PASS |
| TC-004 | Pop/top on empty stack | Throws `std::underflow_error` | Threw underflow_error | PASS |
| TC-005 | Push `Process*` (P1, P2) | Popped order: P2, then P1 | P2, then P1 | PASS |
| TC-006 | Clear | `isEmpty()=true`, `size()=0` | `true`, `0` | PASS |

## Known Limitations

- `Stack` will be integrated with CPU execution tracking during Phase 12 (Execution History).

## Next Step

**Phase 2 Complete!**
Proceed to **Phase 3: Searching and Sorting** — starting with **Task 3.1: Custom Sorting Algorithms** (`src/utils/SearchSort.h` - sorting processes by Arrival Time, Burst Time, and Priority without `std::sort`).

## Codebase Notes for Future Agents

- `Stack` will store CPU execution steps (`Process*` or PID execution logs) for LIFO playback.
- Always pass data structures by reference (`Stack<T>&`) as copy operations are disabled.
