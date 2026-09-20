#include <iostream>
#include "core/ProcessManager.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "       DSA CPU SCHEDULER SIMULATOR      \n";
    std::cout << "========================================\n\n";

    std::cout << "--- Process Manager & Input Validation Verification ---\n";

    ProcessManager manager;

    std::cout << "\n1. Adding valid standard processes (Section 24 Benchmark Workload)...\n";
    manager.addProcess(1, 0, 5, 2); // P1
    manager.addProcess(2, 1, 3, 1); // P2
    manager.addProcess(3, 2, 8, 3); // P3

    std::cout << "\n2. Testing Input Validation Error Cases:\n";
    
    std::cout << "   a. Testing Duplicate PID (P1): ";
    manager.addProcess(1, 4, 6, 2);

    std::cout << "   b. Testing Negative Arrival Time (-2): ";
    manager.addProcess(4, -2, 5, 1);

    std::cout << "   c. Testing Non-positive Burst Time (0): ";
    manager.addProcess(5, 3, 0, 1);

    std::cout << "   d. Testing Non-positive Priority (0): ";
    manager.addProcess(6, 3, 4, 0);

    std::cout << "\n3. Displaying Loaded Process Workload:\n";
    manager.displayAll();

    return 0;
}
