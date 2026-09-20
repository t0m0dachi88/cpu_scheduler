#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

#include "core/Process.h"

// Enumeration of supported sorting keys for process workloads
enum class ProcessSortKey {
    ARRIVAL_TIME,
    BURST_TIME,
    PRIORITY,
    PID
};

class SearchSort {
private:
    // Internal helper for Merge Sort merging step
    static void merge(Process* arr, int left, int mid, int right, ProcessSortKey key);
    
    // Internal recursive Merge Sort function
    static void mergeSortHelper(Process* arr, int left, int right, ProcessSortKey key);

    // Comparator helper returning true if process 'a' precedes process 'b' under the given key
    static bool compareProcesses(const Process& a, const Process& b, ProcessSortKey key);

public:
    // Sorts an array of processes using custom Merge Sort - O(n log n) Time, O(n) Space
    static void mergeSort(Process* processes, int count, ProcessSortKey key);

    // Sorts an array of processes using custom Insertion Sort - O(n^2) Time, O(1) Space
    static void insertionSort(Process* processes, int count, ProcessSortKey key);

    // Convenience sorting functions
    static void sortByArrivalTime(Process* processes, int count);
    static void sortByBurstTime(Process* processes, int count);
    static void sortByPriority(Process* processes, int count);
    static void sortByPID(Process* processes, int count);

    // Linear Search for PID lookup - O(n) Time
    static int linearSearchByPID(const Process* processes, int count, int targetPID);

    // Binary Search for PID lookup - O(log n) Time (Requires array to be sorted by PID)
    static int binarySearchByPID(const Process* processes, int count, int targetPID);

    // Helper verifying whether process array is sorted by PID
    static bool isSortedByPID(const Process* processes, int count);
};

#endif // SEARCH_SORT_H
