#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "core/Process.h"
#include <string>

// Manages a dynamic collection of Process instances for scheduling workloads.
class ProcessManager {
private:
    Process* processes; // Dynamic array storing processes
    int capacity;       // Allocated capacity of the process array
    int count;          // Current number of processes stored

    // Helper method to resize the dynamic process array
    void resize(int newCapacity);

public:
    explicit ProcessManager(int initialCapacity = 10);
    ~ProcessManager();

    // Copy constructor and assignment operator disabled to enforce unique ownership
    ProcessManager(const ProcessManager&) = delete;
    ProcessManager& operator=(const ProcessManager&) = delete;

    // Adds a process after validating inputs
    bool addProcess(int pid, int arrivalTime, int burstTime, int priority = 1);
    bool addProcess(const Process& p);

    // Validates if input parameters are within valid bounds and PID is unique
    bool validateProcessInput(int pid, int arrivalTime, int burstTime, int priority, std::string& errorMsg) const;

    // Checks if a Process ID already exists in the collection
    bool existsPID(int pid) const;

    // Interactive console input method
    void inputProcessFromConsole();

    // Displays formatted table of all loaded processes
    void displayAll() const;

    // Resets dynamic state of all processes for re-simulations
    void resetAll();

    // Accessors
    int getCount() const { return count; }
    const Process* getProcesses() const { return processes; }
    Process* getProcessesMutable() { return processes; }
    Process getProcessAt(int index) const;

    // Clears all stored processes
    void clear();
};

#endif // PROCESS_MANAGER_H
