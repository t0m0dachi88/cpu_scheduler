# Task: Circular Queue Implementation

## Status

Completed

## Objective

Implement a template-based `CircularQueue` data structure (`src/dsa/CircularQueue.h`) using an array-based circular buffer with modulo wrap-around indexing and automatic buffer expansion, designed specifically for process rotation in **Round Robin Scheduling**.

## What Was Implemented

1. `CircularQueue<T>` Class:
   - Dynamic array buffer (`buffer`), `frontIdx`, `rearIdx`, `capacity`, and `count`.
   - `enqueue(const T& value)`: Enqueues element at `(rearIdx + 1) % capacity`. Automatically triggers `resize(capacity * 2)` when buffer is full.
   - `dequeue()`: Removes element at `(frontIdx + 1) % capacity` in \(\mathcal{O}(1)\) time; throws `std::underflow_error` on empty queue.
   - `rotate()`: Rotates front process directly to the back of the queue (`enqueue(dequeue())`) in \(\mathcal{O}(1)\) time.
   - `front()`: Peeks front element without removal in \(\mathcal{O}(1)\) time.
   - `isEmpty()`, `isFull()`, `size()`, `getCapacity()`, `clear()`.
   - Dynamic buffer realignment inside `resize()`.
2. Dedicated Unit Test Suite (`tests/test_circular_queue.cpp`):
   - 6 test cases verifying wrap-around indexing, Round Robin process rotation, dynamic buffer resizing, exception handling, and `Process*` pointer queueing.
3. Makefile Integration:
   - Added `test_circular_queue.exe` target to `make test`.
4. Test Report:
   - Created `docs/testing/test_circular_queue_results.md`.

## How It Works

`CircularQueue<T>` allocates a dynamic array `buffer`. Elements are inserted at `rearIdx` and extracted from `frontIdx`, wrapping around indices using modulo operations `(index + 1) % capacity`. When the buffer reaches maximum capacity, `resize()` allocates a new buffer of double capacity, realigns all `count` elements starting from index 0, resets `frontIdx = 0` and `rearIdx = count - 1`, and deallocates the old buffer.

## DSA / CS Concepts Used

- **Circular Queue Data Structure**: Ring buffer utilizing modulo arithmetic for index wrapping.
- **Round Robin Process Rotation**: Direct \(\mathcal{O}(1)\) process context rotation (`rotate()`).
- **Dynamic Array Realignment**: Re-aligning circular indices to standard linear offsets during array resizing.
- **Generic C++ Templates**: Supporting primitives and `Process*` domain entity pointers.

## Files Created

- `src/dsa/CircularQueue.h`
- `tests/test_circular_queue.cpp`
- `docs/testing/test_circular_queue_results.md`
- `docs/implementation/006-circular-queue.md`

## Files Modified

- `Makefile`

## Important Classes / Functions

- `class CircularQueue<T>`: Array-based circular queue class.
- `CircularQueue::rotate()`: Rotates front process to back of queue.
- `CircularQueue::enqueue()`: Wrap-around enqueue with automatic resize.
- `CircularQueue::dequeue()`: Wrap-around dequeue.

## Design Decisions

- **Circular Array vs Linked List**: Selected array-based circular buffer with modulo indexing for `CircularQueue` to provide cache-friendly contiguous memory locality and efficient \(\mathcal{O}(1)\) rotation for Round Robin time-quantum context switching.

## Testing

1. Executed `make test`.
2. Verified all 6 `CircularQueue` tests passed (`6 / 6 PASSED`).
3. Verified total suite pass rate (`20 / 20 PASSED` across process, queue, and circular queue test suites).

## Test Cases

| Test Case | Input / Operation | Expected Output | Actual Output | Status |
|---|---|---|---|---|
| TC-001 | Initial state (cap=5) | `isEmpty()=true`, `size()=0`, `getCapacity()=5` | `true`, `0`, `5` | PASS |
| TC-002 | Enqueue 10,20,30, Dequeue 2, Enqueue 40,50 | Wrap-around indices dequeue 30,40,50 | 30, 40, 50 | PASS |
| TC-003 | Enqueue P1,P2,P3 & `rotate()` | Front changes to P2; P1 dequeued last | P2 front, P1 last | PASS |
| TC-004 | Enqueue beyond initial capacity (2 -> 3) | Capacity doubles to 4 | Capacity = 4 | PASS |
| TC-005 | Empty dequeue/front | Throws `std::underflow_error` | Threw underflow_error | PASS |
| TC-006 | Enqueue `Process*` | Enqueues and rotates process pointers | Pointers matched | PASS |

## Known Limitations

- Min Heap for Priority Scheduling will be implemented in Task 2.5.

## Next Step

**Phase 2 Task 2.5**: Implement `MinHeap` (`src/dsa/MinHeap.h`) — custom Min Heap data structure for **Non-preemptive Priority Scheduling**.

## Codebase Notes for Future Agents

- `CircularQueue` will serve as the ready queue for **Round Robin Scheduler** (Phase 6).
- Use `rotate()` when a process exhausts its time quantum without finishing.
