#include <iostream>
#include <cassert>
#include <stdexcept>
#include "dsa/MinHeap.h"
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

// Comparator function for Process pointers in Priority Scheduling
// Rule: Lower priority number = higher priority; tie-break by arrival time, then PID.
bool processPriorityComparator(Process* const& a, Process* const& b) {
    if (a->priority != b->priority) {
        return a->priority < b->priority;
    }
    if (a->arrivalTime != b->arrivalTime) {
        return a->arrivalTime < b->arrivalTime;
    }
    return a->id < b->id;
}

bool testMinHeapInitialState() {
    MinHeap<int> heap(5);
    ASSERT_TRUE(heap.isEmpty(), "Initial heap should be empty");
    ASSERT_EQUAL(heap.size(), 0, "Initial size should be 0");
    ASSERT_EQUAL(heap.getCapacity(), 5, "Initial capacity should be 5");
    return true;
}

bool testMinHeapIntegerOrdering() {
    MinHeap<int> heap;
    heap.insert(50);
    heap.insert(20);
    heap.insert(40);
    heap.insert(10);
    heap.insert(30);

    ASSERT_EQUAL(heap.size(), 5, "Size should be 5");
    ASSERT_EQUAL(heap.peekMin(), 10, "Min element should be 10");

    ASSERT_EQUAL(heap.extractMin(), 10, "1st min should be 10");
    ASSERT_EQUAL(heap.extractMin(), 20, "2nd min should be 20");
    ASSERT_EQUAL(heap.extractMin(), 30, "3rd min should be 30");
    ASSERT_EQUAL(heap.extractMin(), 40, "4th min should be 40");
    ASSERT_EQUAL(heap.extractMin(), 50, "5th min should be 50");
    ASSERT_TRUE(heap.isEmpty(), "Heap should be empty after extracting all");
    return true;
}

bool testMinHeapDuplicatePriorities() {
    MinHeap<int> heap;
    heap.insert(5);
    heap.insert(2);
    heap.insert(5);
    heap.insert(2);
    heap.insert(1);

    ASSERT_EQUAL(heap.extractMin(), 1, "1st min should be 1");
    ASSERT_EQUAL(heap.extractMin(), 2, "2nd min should be 2");
    ASSERT_EQUAL(heap.extractMin(), 2, "3rd min should be 2");
    ASSERT_EQUAL(heap.extractMin(), 5, "4th min should be 5");
    ASSERT_EQUAL(heap.extractMin(), 5, "5th min should be 5");
    return true;
}

bool testMinHeapExceptions() {
    MinHeap<int> heap;
    bool exceptionCaught = false;
    try {
        heap.extractMin();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty extractMin should throw std::underflow_error");

    exceptionCaught = false;
    try {
        heap.peekMin();
    } catch (const std::underflow_error& e) {
        exceptionCaught = true;
    }
    ASSERT_TRUE(exceptionCaught, "Empty peekMin should throw std::underflow_error");
    return true;
}

bool testMinHeapDynamicResizing() {
    MinHeap<int> heap(2); // Small initial capacity = 2
    heap.insert(100);
    heap.insert(50);
    ASSERT_EQUAL(heap.getCapacity(), 2, "Capacity should be 2");

    // Insert 3rd element to force array expansion
    heap.insert(25);
    ASSERT_EQUAL(heap.getCapacity(), 4, "Capacity should double to 4");
    ASSERT_EQUAL(heap.size(), 3, "Size should be 3");
    ASSERT_EQUAL(heap.peekMin(), 25, "Min element should be 25");
    return true;
}

bool testMinHeapProcessPointersWithPriorityComparator() {
    MinHeap<Process*> priorityHeap(10, processPriorityComparator);

    Process p1(1, 0, 5, 2); // PID=1, AT=0, BT=5, Prio=2
    Process p2(2, 1, 3, 1); // PID=2, AT=1, BT=3, Prio=1 (Highest priority)
    Process p3(3, 2, 8, 3); // PID=3, AT=2, BT=8, Prio=3 (Lowest priority)
    Process p4(4, 0, 4, 1); // PID=4, AT=0, BT=4, Prio=1 (Earlier arrival tie break)

    priorityHeap.insert(&p1);
    priorityHeap.insert(&p2);
    priorityHeap.insert(&p3);
    priorityHeap.insert(&p4);

    ASSERT_EQUAL(priorityHeap.size(), 4, "Heap size should be 4");

    // P4 and P2 both have priority 1, but P4 arrived at AT=0 whereas P2 arrived at AT=1
    Process* first = priorityHeap.extractMin();
    ASSERT_EQUAL(first->id, 4, "First extracted process should be P4 (Prio 1, AT 0)");

    Process* second = priorityHeap.extractMin();
    ASSERT_EQUAL(second->id, 2, "Second extracted process should be P2 (Prio 1, AT 1)");

    Process* third = priorityHeap.extractMin();
    ASSERT_EQUAL(third->id, 1, "Third extracted process should be P1 (Prio 2)");

    Process* fourth = priorityHeap.extractMin();
    ASSERT_EQUAL(fourth->id, 3, "Fourth extracted process should be P3 (Prio 3)");

    ASSERT_TRUE(priorityHeap.isEmpty(), "Heap should be empty");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     RUNNING MIN HEAP UNIT TESTS        \n";
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

    runTest("MinHeap Initial State", testMinHeapInitialState);
    runTest("MinHeap Integer Ordering (Heapify Up & Down)", testMinHeapIntegerOrdering);
    runTest("MinHeap Duplicate Priorities Handling", testMinHeapDuplicatePriorities);
    runTest("MinHeap Underflow Exception Handling", testMinHeapExceptions);
    runTest("MinHeap Dynamic Resizing Buffer", testMinHeapDynamicResizing);
    runTest("MinHeap Process Pointer Priority Comparator", testMinHeapProcessPointersWithPriorityComparator);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
