#include <iostream>
#include <cassert>
#include <stdexcept>
#include "dsa/CustomQueue.h"
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

bool testQueueEmptyState() {
    CustomQueue<int> q;
    ASSERT_TRUE(q.isEmpty(), "New queue should be empty");
    ASSERT_EQUAL(q.size(), 0, "New queue size should be 0");
    return true;
}

bool testQueueEnqueueAndFront() {
    CustomQueue<int> q;
    q.enqueue(10);
    ASSERT_TRUE(!q.isEmpty(), "Queue should not be empty after enqueue");
    ASSERT_EQUAL(q.size(), 1, "Queue size should be 1");
    ASSERT_EQUAL(q.front(), 10, "Front element should be 10");

    q.enqueue(20);
    ASSERT_EQUAL(q.size(), 2, "Queue size should be 2");
    ASSERT_EQUAL(q.front(), 10, "Front element should still be 10 (FIFO)");
    return true;
}

bool testQueueFifoOrder() {
    CustomQueue<int> q;
    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);

    ASSERT_EQUAL(q.dequeue(), 100, "First dequeued should be 100");
    ASSERT_EQUAL(q.dequeue(), 200, "Second dequeued should be 200");
    ASSERT_EQUAL(q.dequeue(), 300, "Third dequeued should be 300");
    ASSERT_TRUE(q.isEmpty(), "Queue should be empty after dequeuing all");
    return true;
}

bool testQueueUnderflowExceptions() {
    CustomQueue<int> q;
    bool exceptionCaught = false;
    try {
        q.dequeue();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Dequeueing from empty queue should throw std::underflow_error");

    exceptionCaught = false;
    try {
        q.front();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Accessing front of empty queue should throw std::underflow_error");
    return true;
}

bool testQueueWithProcessPointers() {
    CustomQueue<Process*> q;
    Process p1(1, 0, 5, 2);
    Process p2(2, 1, 3, 1);

    q.enqueue(&p1);
    q.enqueue(&p2);

    ASSERT_EQUAL(q.size(), 2, "Size should be 2");
    ASSERT_EQUAL(q.front()->id, 1, "Front process PID should be 1");
    ASSERT_EQUAL(q.dequeue()->id, 1, "Dequeued process PID should be 1");
    ASSERT_EQUAL(q.front()->id, 2, "Next front process PID should be 2");
    ASSERT_EQUAL(q.dequeue()->id, 2, "Dequeued process PID should be 2");
    ASSERT_TRUE(q.isEmpty(), "Queue should be empty");
    return true;
}

bool testQueueClear() {
    CustomQueue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    q.clear();
    ASSERT_TRUE(q.isEmpty(), "Cleared queue should be empty");
    ASSERT_EQUAL(q.size(), 0, "Cleared queue size should be 0");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "      RUNNING CUSTOM QUEUE UNIT TESTS   \n";
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

    runTest("CustomQueue Empty Initial State", testQueueEmptyState);
    runTest("CustomQueue Enqueue & Front Peek", testQueueEnqueueAndFront);
    runTest("CustomQueue FIFO Order Verification", testQueueFifoOrder);
    runTest("CustomQueue Underflow Exception Handling", testQueueUnderflowExceptions);
    runTest("CustomQueue Process Pointer Support", testQueueWithProcessPointers);
    runTest("CustomQueue Clear & Memory Release", testQueueClear);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
