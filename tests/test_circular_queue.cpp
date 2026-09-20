#include <iostream>
#include <cassert>
#include <stdexcept>
#include "dsa/CircularQueue.h"
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

bool testCircularQueueInitialState() {
    CircularQueue<int> cq(5);
    ASSERT_TRUE(cq.isEmpty(), "Initial queue should be empty");
    ASSERT_EQUAL(cq.size(), 0, "Initial size should be 0");
    ASSERT_EQUAL(cq.getCapacity(), 5, "Initial capacity should be 5");
    return true;
}

bool testCircularQueueEnqueueDequeueWrapAround() {
    CircularQueue<int> cq(3);
    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);

    ASSERT_TRUE(cq.isFull(), "Queue should be full with 3 elements");
    ASSERT_EQUAL(cq.dequeue(), 10, "First dequeued should be 10");
    ASSERT_EQUAL(cq.dequeue(), 20, "Second dequeued should be 20");

    // Wrap around enqueue
    cq.enqueue(40);
    cq.enqueue(50);

    ASSERT_EQUAL(cq.front(), 30, "Front element should be 30");
    ASSERT_EQUAL(cq.dequeue(), 30, "Dequeued should be 30");
    ASSERT_EQUAL(cq.dequeue(), 40, "Dequeued should be 40");
    ASSERT_EQUAL(cq.dequeue(), 50, "Dequeued should be 50");
    ASSERT_TRUE(cq.isEmpty(), "Queue should be empty");
    return true;
}

bool testCircularQueueRotation() {
    CircularQueue<int> cq;
    cq.enqueue(1); // P1
    cq.enqueue(2); // P2
    cq.enqueue(3); // P3

    // Rotate front element (P1) to back
    cq.rotate();
    ASSERT_EQUAL(cq.front(), 2, "Front after rotation should be P2");
    ASSERT_EQUAL(cq.dequeue(), 2, "First dequeued should be P2");
    ASSERT_EQUAL(cq.dequeue(), 3, "Second dequeued should be P3");
    ASSERT_EQUAL(cq.dequeue(), 1, "Rotated P1 should be dequeued last");
    return true;
}

bool testCircularQueueBufferResize() {
    CircularQueue<int> cq(2);
    cq.enqueue(1);
    cq.enqueue(2);

    ASSERT_EQUAL(cq.getCapacity(), 2, "Capacity should initially be 2");

    // Enqueue 3rd element to force capacity doubling
    cq.enqueue(3);
    ASSERT_EQUAL(cq.getCapacity(), 4, "Capacity should double to 4");
    ASSERT_EQUAL(cq.size(), 3, "Size should be 3");

    ASSERT_EQUAL(cq.dequeue(), 1, "Dequeued 1");
    ASSERT_EQUAL(cq.dequeue(), 2, "Dequeued 2");
    ASSERT_EQUAL(cq.dequeue(), 3, "Dequeued 3");
    return true;
}

bool testCircularQueueExceptions() {
    CircularQueue<int> cq;
    bool exceptionCaught = false;
    try {
        cq.dequeue();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty dequeue should throw std::underflow_error");

    exceptionCaught = false;
    try {
        cq.front();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty front access should throw std::underflow_error");
    return true;
}

bool testCircularQueueProcessPointers() {
    CircularQueue<Process*> cq;
    Process p1(1, 0, 5, 2);
    Process p2(2, 1, 3, 1);

    cq.enqueue(&p1);
    cq.enqueue(&p2);

    ASSERT_EQUAL(cq.front()->id, 1, "Front process PID should be 1");
    cq.rotate();
    ASSERT_EQUAL(cq.front()->id, 2, "Front process PID after rotation should be 2");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   RUNNING CIRCULAR QUEUE UNIT TESTS    \n";
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

    runTest("CircularQueue Initial State", testCircularQueueInitialState);
    runTest("CircularQueue Enqueue/Dequeue Wrap-Around", testCircularQueueEnqueueDequeueWrapAround);
    runTest("CircularQueue Rotation (Round Robin)", testCircularQueueRotation);
    runTest("CircularQueue Buffer Resize", testCircularQueueBufferResize);
    runTest("CircularQueue Underflow Exceptions", testCircularQueueExceptions);
    runTest("CircularQueue Process Pointer Support", testCircularQueueProcessPointers);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
