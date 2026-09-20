#include <iostream>
#include <iomanip>
#include "core/Process.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "       DSA CPU SCHEDULER SIMULATOR      \n";
    std::cout << "========================================\n\n";

    std::cout << "--- Process Model Verification ---\n";
    
    // Sample test workload from Section 24 of prompt
    Process p1(1, 0, 5, 2);
    Process p2(2, 1, 3, 1);
    Process p3(3, 2, 8, 3);

    // Simulate completion times for manual test check
    p1.completionTime = 5;
    p1.calculateMetrics();
    p1.state = ProcessState::COMPLETED;

    p2.completionTime = 8;
    p2.calculateMetrics();
    p2.state = ProcessState::COMPLETED;

    p3.completionTime = 16;
    p3.calculateMetrics();
    p3.state = ProcessState::COMPLETED;

    std::cout << "+-------+----------+--------+----------+--------+--------+--------+------------+\n";
    std::cout << "|   PID | Arrival  | Burst  | Priority |   CT   |  TAT   |   WT   |    State   |\n";
    std::cout << "+-------+----------+--------+----------+--------+--------+--------+------------+\n";

    p1.printProcessDetails();
    p2.printProcessDetails();
    p3.printProcessDetails();

    std::cout << "+-------+----------+--------+----------+--------+--------+--------+------------+\n";

    return 0;
}
