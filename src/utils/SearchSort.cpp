#include "utils/SearchSort.h"
#include <iostream>

bool SearchSort::compareProcesses(const Process& a, const Process& b, ProcessSortKey key) {
    switch (key) {
        case ProcessSortKey::ARRIVAL_TIME:
            if (a.arrivalTime != b.arrivalTime) return a.arrivalTime < b.arrivalTime;
            return a.id < b.id;
        case ProcessSortKey::BURST_TIME:
            if (a.burstTime != b.burstTime) return a.burstTime < b.burstTime;
            return a.arrivalTime < b.arrivalTime;
        case ProcessSortKey::PRIORITY:
            if (a.priority != b.priority) return a.priority < b.priority; // Lower number = higher priority
            return a.arrivalTime < b.arrivalTime;
        case ProcessSortKey::PID:
            return a.id < b.id;
        default:
            return a.id < b.id;
    }
}

void SearchSort::insertionSort(Process* processes, int count, ProcessSortKey key) {
    if (processes == nullptr || count <= 1) return;

    for (int i = 1; i < count; ++i) {
        Process keyProcess = processes[i];
        int j = i - 1;
        while (j >= 0 && compareProcesses(keyProcess, processes[j], key)) {
            processes[j + 1] = processes[j];
            j--;
        }
        processes[j + 1] = keyProcess;
    }
}

void SearchSort::merge(Process* arr, int left, int mid, int right, ProcessSortKey key) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Process* L = new Process[n1];
    Process* R = new Process[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (compareProcesses(L[i], R[j], key) || (!compareProcesses(R[j], L[i], key) && L[i].id <= R[j].id)) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void SearchSort::mergeSortHelper(Process* arr, int left, int right, ProcessSortKey key) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, key);
        mergeSortHelper(arr, mid + 1, right, key);
        merge(arr, left, mid, right, key);
    }
}

void SearchSort::mergeSort(Process* processes, int count, ProcessSortKey key) {
    if (processes == nullptr || count <= 1) return;
    mergeSortHelper(processes, 0, count - 1, key);
}

void SearchSort::sortByArrivalTime(Process* processes, int count) {
    mergeSort(processes, count, ProcessSortKey::ARRIVAL_TIME);
}

void SearchSort::sortByBurstTime(Process* processes, int count) {
    mergeSort(processes, count, ProcessSortKey::BURST_TIME);
}

void SearchSort::sortByPriority(Process* processes, int count) {
    mergeSort(processes, count, ProcessSortKey::PRIORITY);
}

void SearchSort::sortByPID(Process* processes, int count) {
    mergeSort(processes, count, ProcessSortKey::PID);
}

int SearchSort::linearSearchByPID(const Process* processes, int count, int targetPID) {
    if (processes == nullptr || count <= 0) return -1;
    for (int i = 0; i < count; ++i) {
        if (processes[i].id == targetPID) {
            return i;
        }
    }
    return -1;
}

bool SearchSort::isSortedByPID(const Process* processes, int count) {
    if (processes == nullptr || count <= 1) return true;
    for (int i = 0; i < count - 1; ++i) {
        if (processes[i].id > processes[i + 1].id) {
            return false;
        }
    }
    return true;
}

int SearchSort::binarySearchByPID(const Process* processes, int count, int targetPID) {
    if (processes == nullptr || count <= 0) return -1;
    if (!isSortedByPID(processes, count)) {
        std::cout << "[Warning]: Binary search requested on process list not sorted by PID. Falling back to linear search.\n";
        return linearSearchByPID(processes, count, targetPID);
    }

    int low = 0;
    int high = count - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (processes[mid].id == targetPID) {
            return mid;
        } else if (processes[mid].id < targetPID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
