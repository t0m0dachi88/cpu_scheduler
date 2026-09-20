# Task: Custom Queue Implementation

## Status

Completed

## Objective

Implement a template-based `CustomQueue` data structure (`src/dsa/CustomQueue.h`) using linked nodes built strictly from scratch without C++ STL containers (`std::queue`, `std::list`, `std::vector`), and test its functionality using a dedicated test suite (`tests/test_queue.cpp`).

## What Was Implemented

1. `QueueNode<T>` struct:
   - Contains data of generic type `T` and pointer to next node (`QueueNode* next`).
2. `CustomQueue<T>` Class:
   - Generic FIFO Queue built with singly linked nodes (`frontNode`, `rearNode`, `count`).
   - `enqueue(const T& value)`: Appends element to rear in \(\mathcal{O}(1)\) time.
   - `dequeue()`: Removes and returns element from front in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty queue.
   - `front()`: Peeks at front element without removal in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty queue.
   - `isEmpty()`: Checks whether count is 0 in \(\mathcal{O}(1)\) time.
   - `size()`: Returns total number of elements in \(\mathcal{O}(1)\) time.
   - `clear()`: Iteratively deallocates nodes in \(\mathcal{O}(n)\) time.
   - Destructor: Invokes `clear()` for automatic RAII memory cleanup.
3. Dedicated Unit Test Suite (`tests/test_queue.cpp`):
   - 6 test cases verifying FIFO order, exception throwing, pointer support (`Process*`), empty state, and memory cleanup.
4. Makefile Integration:
   - Added `test_queue.exe` target to `make test`.
5. Test Report:
   - Created `docs/testing/test_queue_results.md`.

## How It Works

`CustomQueue<T>` maintains `frontNode` and `rearNode` pointers. Enqueuing creates a new `QueueNode<T>` on the heap and links `rearNode->next` to it before advancing `rearNode`. Dequeuing unlinks `frontNode`, advances `frontNode`, deallocates the unlinked node using `delete`, and decrements `count`.

## DSA / CS Concepts Used

- **Queue Data Structure (FIFO)**: First-In, First-Out dynamic data structure.
- **Singly Linked Node Allocation**: Pointer-based dynamic memory allocation on heap avoiding contiguous array re-allocation overhead.
- **Generic Programming (C++ Templates)**: Allowing `CustomQueue` to store primitives (`int`) as well as domain entity pointers (`Process*`).
- **Resource Management (RAII)**: Linked list node cleanup in destructor preventing memory leaks.

## Files Created

- `src/dsa/CustomQueue.h`
- `tests/test_queue.cpp`
- `docs/testing/test_queue_results.md`
- `docs/implementation/005-custom-queue.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `struct QueueNode<T>`: Node representation.
- `class CustomQueue<T>`: Custom FIFO queue class.
- `CustomQueue::enqueue()`: Enqueues item at tail.
- `CustomQueue::dequeue()`: Removes item from head.
- `CustomQueue::front()`: Peeks at head item.

## Design Decisions

- **Linked List vs Array**: Selected pointer-linked nodes over circular array buffer for `CustomQueue` to provide unbounded dynamic queue size without reallocation copy penalties, maintaining strict \(\mathcal{O}(1)\) time complexity for all operations.
- **Template Design**: Templating `CustomQueue<T>` allows `Process*` pointers to be enqueued directly during FCFS CPU scheduling.

## Testing

1. Executed `make test`.
2. Verified all 6 `CustomQueue` tests passed (`6 / 6 PASSED`).
3. Verified zero regressions in `Process` unit tests (`8 / 8 PASSED`).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Initial state | `isEmpty()=true`, `size()=0` | `true`, `0` | PASS |
| TC-002 | Enqueue 10, 20 | `front()=10`, `size()=2` | `10`, `2` | PASS |
| TC-003 | Enqueue 100, 200, 300 & Dequeue 3x | 100, then 200, then 300 | 100, 200, 300 | PASS |
| TC-004 | Dequeue on empty queue | Throws `std::underflow_error` | Threw underflow_error | PASS |
| TC-005 | Enqueue `Process*` | Enqueues and dequeues process pointers | Pointers matched | PASS |
| TC-006 | Clear | `isEmpty()=true`, `size()=0` | `true`, `0` | PASS |

## Known Limitations

- `CustomQueue` is linear FIFO; rotation for Round Robin will use `CircularQueue` (Phase 2 Task 2.3).

## Next Step

**Phase 2 Task 2.3**: Implement `CircularQueue` (`src/dsa/CircularQueue.h`) for Round Robin process rotation.

## Codebase Notes for Future Agents

- `CustomQueue` will be used directly as the ready queue for **FCFS Scheduler** (Phase 5).
- Never include `<queue>` inside `CustomQueue.h` or scheduler files.
