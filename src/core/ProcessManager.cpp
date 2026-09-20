#include "core/ProcessManager.h"
#include <iostream>
#include <iomanip>

ProcessManager::ProcessManager(int initialCapacity)
    : processes(nullptr), capacity(initialCapacity > 0 ? initialCapacity : 10), count(0) {
    processes = new Process[capacity];
}

ProcessManager::~ProcessManager() {
    delete[] processes;
    processes = nullptr;
}

void ProcessManager::resize(int newCapacity) {
    Process* newArr = new Process[newCapacity];
    for (int i = 0; i < count; ++i) {
        newArr[i] = processes[i];
    }
    delete[] processes;
    processes = newArr;
    capacity = newCapacity;
}

bool ProcessManager::existsPID(int pid) const {
    for (int i = 0; i < count; ++i) {
        if (processes[i].id == pid) {
            return true;
        }
    }
    return false;
}

bool ProcessManager::validateProcessInput(int pid, int arrivalTime, int burstTime, int priority, std::string& errorMsg) const {
    if (pid <= 0) {
        errorMsg = "Process ID (PID) must be a positive integer (> 0).";
        return false;
    }
    if (existsPID(pid)) {
        errorMsg = "Process ID " + std::to_string(pid) + " already exists. PIDs must be unique.";
        return false;
    }
    if (arrivalTime < 0) {
        errorMsg = "Arrival time cannot be negative.";
        return false;
    }
    if (burstTime <= 0) {
        errorMsg = "Burst time must be a positive integer (> 0).";
        return false;
    }
    if (priority <= 0) {
        errorMsg = "Priority must be a positive integer (> 0, where 1 is highest priority).";
        return false;
    }
    return true;
}

bool ProcessManager::addProcess(int pid, int arrivalTime, int burstTime, int priority) {
    std::string errorMsg;
    if (!validateProcessInput(pid, arrivalTime, burstTime, priority, errorMsg)) {
        std::cout << "[Error]: " << errorMsg << "\n";
        return false;
    }

    if (count >= capacity) {
        resize(capacity * 2);
    }

    processes[count] = Process(pid, arrivalTime, burstTime, priority);
    count++;
    return true;
}

bool ProcessManager::addProcess(const Process& p) {
    return addProcess(p.id, p.arrivalTime, p.burstTime, p.priority);
}

void ProcessManager::inputProcessFromConsole() {
    int pid, arrival, burst, prio;
    std::cout << "\n--- Add New Process ---\n";
    std::cout << "Enter Process ID (PID): ";
    if (!(std::cin >> pid)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "[Error]: Invalid PID input.\n";
        return;
    }

    std::cout << "Enter Arrival Time (>= 0): ";
    if (!(std::cin >> arrival)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "[Error]: Invalid Arrival Time input.\n";
        return;
    }

    std::cout << "Enter Burst Time (> 0): ";
    if (!(std::cin >> burst)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "[Error]: Invalid Burst Time input.\n";
        return;
    }

    std::cout << "Enter Priority (> 0, lower integer = higher priority): ";
    if (!(std::cin >> prio)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "[Error]: Invalid Priority input.\n";
        return;
    }

    if (addProcess(pid, arrival, burst, prio)) {
        std::cout << "[Success]: Process P" << pid << " added successfully!\n";
    }
}

void ProcessManager::displayAll() const {
    if (count == 0) {
        std::cout << "\n[Info]: No processes currently loaded in the system.\n";
        return;
    }

    std::cout << "\n+-------+----------+--------+----------+--------+--------+--------+------------+\n";
    std::cout << "|   PID | Arrival  | Burst  | Priority |   CT   |  TAT   |   WT   |    State   |\n";
    std::cout << "+-------+----------+--------+----------+--------+--------+--------+------------+\n";

    for (int i = 0; i < count; ++i) {
        processes[i].printProcessDetails();
    }

    std::cout << "+-------+----------+--------+----------+--------+--------+--------+------------+\n";
    std::cout << "Total Processes Loaded: " << count << "\n";
}

void ProcessManager::resetAll() {
    for (int i = 0; i < count; ++i) {
        processes[i].reset();
    }
}

Process ProcessManager::getProcessAt(int index) const {
    if (index >= 0 && index < count) {
        return processes[index];
    }
    return Process();
}

void ProcessManager::clear() {
    count = 0;
}
