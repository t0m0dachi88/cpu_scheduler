#include <iostream>
#include "core/ProcessManager.h"
#include "schedulers/FCFS.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "       DSA CPU SCHEDULER SIMULATOR      \n";
    std::cout << "========================================\n\n";

    ProcessManager manager;

    std::cout << "1. Loading Benchmark Process Workload (Section 24)...\n";
    manager.addProcess(1, 0, 5, 2); // P1: AT=0, BT=5, Priority=2
    manager.addProcess(2, 1, 3, 1); // P2: AT=1, BT=3, Priority=1
    manager.addProcess(3, 2, 8, 3); // P3: AT=2, BT=8, Priority=3

    std::cout << "\n2. Initial Workload State:\n";
    manager.displayAll();

    std::cout << "\n3. Executing First-Come First-Served (FCFS) Scheduler...\n";
    FCFS fcfs;
    fcfs.runSimulation(manager);

    std::cout << "\n4. Displaying FCFS Simulation Results:\n";
    fcfs.printSummary(manager);

    return 0;
}
