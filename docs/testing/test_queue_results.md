# Test Execution Report: CustomQueue Data Structure

## Executive Summary
- **Test Executable**: `test_queue.exe`
- **Source Code under test**: `src/dsa/CustomQueue.h`
- **Total Tests Executed**: 6
- **Tests Passed**: 6
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-QUEUE-001 | Empty Initial State | `CustomQueue` | Verifies `isEmpty()` returns `true` and `size()` returns `0` on instantiation. | **PASS** |
| UT-QUEUE-002 | Enqueue & Front | `CustomQueue` | Verifies `enqueue()` updates size and `front()` peeks without removing element. | **PASS** |
| UT-QUEUE-003 | FIFO Order Verification | `CustomQueue` | Validates First-In, First-Out sequence across multiple `enqueue`/`dequeue` operations. | **PASS** |
| UT-QUEUE-004 | Underflow Exceptions | `CustomQueue` | Validates `std::underflow_error` is thrown on empty `dequeue()` or empty `front()`. | **PASS** |
| UT-QUEUE-005 | Process Pointer Support | `CustomQueue` | Verifies template queue storing `Process*` pointers for FCFS ready queue usage. | **PASS** |
| UT-QUEUE-006 | Clear & Memory Release | `CustomQueue` | Verifies `clear()` deallocates all linked nodes and resets size to `0`. | **PASS** |

---

## Time & Space Complexity

| Operation | Time Complexity | Space Complexity |
|---|---|---|
| `enqueue(item)` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `dequeue()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `front()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `isEmpty()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `size()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `clear()` | \(\mathcal{O}(n)\) | \(\mathcal{O}(1)\) |

---

## Command to Reproduce

```bash
make test
```
