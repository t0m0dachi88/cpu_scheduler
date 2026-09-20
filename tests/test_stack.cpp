#include <iostream>
#include <cassert>
#include <stdexcept>
#include "dsa/Stack.h"
#include "core/Process.h"

#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL]: " << message << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

#define ASSERT_EQUAL(actual, expected, message) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL]: " << message << " (Expected: " << (expected) << ", Got: " << (actual) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

bool testStackInitialState() {
    Stack<int> s;
    ASSERT_TRUE(s.isEmpty(), "Initial stack should be empty");
    ASSERT_EQUAL(s.size(), 0, "Initial size should be 0");
    return true;
}

bool testStackLIFOOrder() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    ASSERT_EQUAL(s.size(), 3, "Stack size should be 3");
    ASSERT_EQUAL(s.top(), 30, "Top element should be 30");

    ASSERT_EQUAL(s.pop(), 30, "First popped element should be 30 (LIFO)");
    ASSERT_EQUAL(s.pop(), 20, "Second popped element should be 20");
    ASSERT_EQUAL(s.pop(), 10, "Third popped element should be 10");
    ASSERT_TRUE(s.isEmpty(), "Stack should be empty after popping all elements");
    return true;
}

bool testStackPeekTop() {
    Stack<int> s;
    s.push(100);
    ASSERT_EQUAL(s.top(), 100, "Top element should be 100");
    ASSERT_EQUAL(s.size(), 1, "Size should remain 1 after top()");
    return true;
}

bool testStackExceptions() {
    Stack<int> s;
    bool exceptionCaught = false;
    try {
        s.pop();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty pop should throw std::underflow_error");

    exceptionCaught = false;
    try {
        s.top();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty top should throw std::underflow_error");
    return true;
}

bool testStackProcessPointers() {
    Stack<Process*> historyStack;
    Process p1(1, 0, 5, 2);
    Process p2(2, 1, 3, 1);

    historyStack.push(&p1);
    historyStack.push(&p2);

    ASSERT_EQUAL(historyStack.top()->id, 2, "Most recent execution on top should be P2");
    ASSERT_EQUAL(historyStack.pop()->id, 2, "First popped execution should be P2");
    ASSERT_EQUAL(historyStack.pop()->id, 1, "Second popped execution should be P1");
    ASSERT_TRUE(historyStack.isEmpty(), "History stack should be empty");
    return true;
}

bool testStackClear() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.clear();
    ASSERT_TRUE(s.isEmpty(), "Cleared stack should be empty");
    ASSERT_EQUAL(s.size(), 0, "Cleared stack size should be 0");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "        RUNNING STACK UNIT TESTS        \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total = 6;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Stack Initial State", testStackInitialState);
    runTest("Stack LIFO Order Verification", testStackLIFOOrder);
    runTest("Stack Peek Top Element", testStackPeekTop);
    runTest("Stack Underflow Exceptions", testStackExceptions);
    runTest("Stack Process Pointer Execution History", testStackProcessPointers);
    runTest("Stack Clear & Memory Deallocation", testStackClear);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
