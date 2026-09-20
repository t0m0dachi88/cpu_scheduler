#include <iostream>
#include <cassert>
#include "utils/SearchSort.h"
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

bool testSortByArrivalTime() {
    Process arr[4] = {
        Process(1, 3, 5, 2),
        Process(2, 0, 3, 1),
        Process(3, 2, 8, 3),
        Process(4, 1, 4, 1)
    };

    SearchSort::sortByArrivalTime(arr, 4);

    ASSERT_EQUAL(arr[0].id, 2, "1st process by arrival should be P2 (AT=0)");
    ASSERT_EQUAL(arr[1].id, 4, "2nd process by arrival should be P4 (AT=1)");
    ASSERT_EQUAL(arr[2].id, 3, "3rd process by arrival should be P3 (AT=2)");
    ASSERT_EQUAL(arr[3].id, 1, "4th process by arrival should be P1 (AT=3)");
    return true;
}

bool testSortByBurstTime() {
    Process arr[4] = {
        Process(1, 0, 8, 2),
        Process(2, 1, 2, 1),
        Process(3, 2, 5, 3),
        Process(4, 3, 1, 1)
    };

    SearchSort::sortByBurstTime(arr, 4);

    ASSERT_EQUAL(arr[0].id, 4, "1st process by burst should be P4 (BT=1)");
    ASSERT_EQUAL(arr[1].id, 2, "2nd process by burst should be P2 (BT=2)");
    ASSERT_EQUAL(arr[2].id, 3, "3rd process by burst should be P3 (BT=5)");
    ASSERT_EQUAL(arr[3].id, 1, "4th process by burst should be P1 (BT=8)");
    return true;
}

bool testSortByPriority() {
    Process arr[4] = {
        Process(1, 0, 5, 3),
        Process(2, 1, 3, 1), // Priority 1 (AT=1)
        Process(3, 2, 8, 2),
        Process(4, 0, 4, 1)  // Priority 1 (AT=0) -> Tie break earlier AT
    };

    SearchSort::sortByPriority(arr, 4);

    ASSERT_EQUAL(arr[0].id, 4, "1st process by priority should be P4 (Prio 1, AT 0)");
    ASSERT_EQUAL(arr[1].id, 2, "2nd process by priority should be P2 (Prio 1, AT 1)");
    ASSERT_EQUAL(arr[2].id, 3, "3rd process by priority should be P3 (Prio 2)");
    ASSERT_EQUAL(arr[3].id, 1, "4th process by priority should be P1 (Prio 3)");
    return true;
}

bool testInsertionSortVSMergeSort() {
    Process arr1[4] = {
        Process(30, 2, 5, 2),
        Process(10, 0, 3, 1),
        Process(40, 3, 8, 3),
        Process(20, 1, 4, 1)
    };
    Process arr2[4] = {
        Process(30, 2, 5, 2),
        Process(10, 0, 3, 1),
        Process(40, 3, 8, 3),
        Process(20, 1, 4, 1)
    };

    SearchSort::insertionSort(arr1, 4, ProcessSortKey::PID);
    SearchSort::mergeSort(arr2, 4, ProcessSortKey::PID);

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQUAL(arr1[i].id, arr2[i].id, "Insertion sort and Merge sort output mismatch");
    }
    return true;
}

bool testLinearSearchByPID() {
    Process arr[3] = {
        Process(10, 0, 5, 2),
        Process(25, 1, 3, 1),
        Process(5, 2, 8, 3)
    };

    int idxFound = SearchSort::linearSearchByPID(arr, 3, 25);
    ASSERT_EQUAL(idxFound, 1, "Linear search should find PID 25 at index 1");

    int idxNotFound = SearchSort::linearSearchByPID(arr, 3, 99);
    ASSERT_EQUAL(idxNotFound, -1, "Linear search for non-existent PID 99 should return -1");
    return true;
}

bool testBinarySearchByPID() {
    Process arr[4] = {
        Process(5, 0, 5, 2),
        Process(10, 1, 3, 1),
        Process(20, 2, 8, 3),
        Process(35, 3, 4, 1)
    }; // Array is pre-sorted by PID

    int idxFound = SearchSort::binarySearchByPID(arr, 4, 20);
    ASSERT_EQUAL(idxFound, 2, "Binary search should find PID 20 at index 2");

    int idxNotFound = SearchSort::binarySearchByPID(arr, 4, 99);
    ASSERT_EQUAL(idxNotFound, -1, "Binary search for non-existent PID 99 should return -1");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "    RUNNING SEARCH & SORT UNIT TESTS    \n";
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

    runTest("SearchSort Sort by Arrival Time", testSortByArrivalTime);
    runTest("SearchSort Sort by Burst Time", testSortByBurstTime);
    runTest("SearchSort Sort by Priority", testSortByPriority);
    runTest("SearchSort Insertion Sort vs Merge Sort Equivalence", testInsertionSortVSMergeSort);
    runTest("SearchSort Linear Search by PID", testLinearSearchByPID);
    runTest("SearchSort Binary Search by PID", testBinarySearchByPID);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
