# Test Execution Report: Stack Data Structure

## Executive Summary
- **Test Executable**: `test_stack.exe`
- **Source Code under test**: `src/dsa/Stack.h`
- **Total Tests Executed**: 6
- **Tests Passed**: 6
- **Tests Failed**: 0
- **Status**: **PASS (100%)**

---

## Detailed Test Results

| Test ID | Test Name | Target Module | Description | Result |
|---|---|---|---|---|
| UT-STACK-001 | Initial State | `Stack` | Verifies initial empty state, size=0, topNode=nullptr. | **PASS** |
| UT-STACK-002 | LIFO Order Verification | `Stack` | Validates Last-In, First-Out extraction order across multiple push/pop operations. | **PASS** |
| UT-STACK-003 | Peek Top Element | `Stack` | Verifies `top()` inspects element without removing it or modifying size. | **PASS** |
| UT-STACK-004 | Underflow Exceptions | `Stack` | Validates `std::underflow_error` on empty `pop()` and empty `top()`. | **PASS** |
| UT-STACK-005 | Execution History Pointers | `Stack` | Verifies `Stack<Process*>` for CPU execution history logging. | **PASS** |
| UT-STACK-006 | Clear & Memory Cleanup | `Stack` | Validates `clear()` deallocates all dynamic nodes and resets size to 0. | **PASS** |

---

## Time & Space Complexity

| Operation | Time Complexity | Space Complexity |
|---|---|---|
| `push(item)` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `pop()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `top()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `isEmpty()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `size()` | \(\mathcal{O}(1)\) | \(\mathcal{O}(1)\) |
| `clear()` | \(\mathcal{O}(n)\) | \(\mathcal{O}(1)\) |

---

## Command to Reproduce

```bash
make test
```
