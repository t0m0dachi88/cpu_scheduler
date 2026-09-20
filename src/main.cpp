#include <iostream>
#include <string>
#include <limits>
#include "core/ProcessManager.h"
#include "schedulers/FCFS.h"
#include "schedulers/RoundRobin.h"
#include "schedulers/PriorityScheduler.h"
#include "utils/Visualizer.h"
#include "utils/PerformanceAnalyzer.h"
#include "utils/FileManager.h"

void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "       DSA CPU SCHEDULER SIMULATOR      \n";
    std::cout << "========================================\n";
    std::cout << "1. Load Workload from File (data/processes.txt)\n";
    std::cout << "2. Input Process Manually\n";
    std::cout << "3. Run First-Come First-Served (FCFS)\n";
    std::cout << "4. Run Round Robin (RR)\n";
    std::cout << "5. Run Priority Scheduling (Non-preemptive)\n";
    std::cout << "6. Run Performance Analyzer (Compare All)\n";
    std::cout << "7. Display Current Process Workload\n";
    std::cout << "8. Clear All Processes\n";
    std::cout << "9. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

void clearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    ProcessManager manager;
    bool running = true;

    while (running) {
        printMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearCin();
            continue;
        }

        switch (choice) {
            case 1: {
                std::string dataFile = "data/processes.txt";
                std::cout << "\nLoading Process Workload from " << dataFile << "...\n";
                if (!FileManager::loadProcessesFromFile(dataFile, manager)) {
                    std::cout << "   [Error] Could not load from " << dataFile << "\n";
                } else {
                    std::cout << "   [Success] Processes loaded successfully.\n";
                    manager.displayAll();
                }
                break;
            }
            case 2: {
                manager.inputProcessFromConsole();
                break;
            }
            case 3: {
                if (manager.getCount() == 0) {
                    std::cout << "\n[Error] No processes loaded. Please load or add processes first.\n";
                    break;
                }
                std::cout << "\nExecuting First-Come First-Served (FCFS)...\n";
                FCFS fcfs;
                fcfs.runSimulation(manager);
                Visualizer::renderGanttChart(fcfs, manager);
                Visualizer::renderProcessTable(manager);
                fcfs.getStateTracker().printTransitionLog();
                break;
            }
            case 4: {
                if (manager.getCount() == 0) {
                    std::cout << "\n[Error] No processes loaded. Please load or add processes first.\n";
                    break;
                }
                int quantum;
                std::cout << "Enter Time Quantum (e.g., 2): ";
                if (!(std::cin >> quantum) || quantum <= 0) {
                    std::cout << "Invalid quantum. Using default = 2.\n";
                    clearCin();
                    quantum = 2;
                }
                std::cout << "\nExecuting Round Robin (Quantum = " << quantum << ")...\n";
                RoundRobin rr(quantum);
                rr.runSimulation(manager);
                Visualizer::renderGanttChart(rr, manager);
                Visualizer::renderProcessTable(manager);
                rr.getStateTracker().printTransitionLog();
                break;
            }
            case 5: {
                if (manager.getCount() == 0) {
                    std::cout << "\n[Error] No processes loaded. Please load or add processes first.\n";
                    break;
                }
                int aging;
                std::cout << "Enter Aging Interval (e.g., 5, or 9999 for no aging): ";
                if (!(std::cin >> aging) || aging <= 0) {
                    std::cout << "Invalid interval. Using default = 5.\n";
                    clearCin();
                    aging = 5;
                }
                std::cout << "\nExecuting Priority Scheduling (Aging = " << aging << ")...\n";
                PriorityScheduler ps(aging);
                ps.runSimulation(manager);
                Visualizer::renderGanttChart(ps, manager);
                Visualizer::renderProcessTable(manager);
                ps.getStateTracker().printTransitionLog();
                break;
            }
            case 6: {
                if (manager.getCount() == 0) {
                    std::cout << "\n[Error] No processes loaded. Please load or add processes first.\n";
                    break;
                }
                int quantum;
                std::cout << "Enter Time Quantum for RR comparison (e.g., 2): ";
                if (!(std::cin >> quantum) || quantum <= 0) {
                    std::cout << "Invalid quantum. Using default = 2.\n";
                    clearCin();
                    quantum = 2;
                }
                std::cout << "\nExecuting Performance Analyzer...\n";
                PerformanceAnalyzer analyzer(quantum);
                analyzer.analyze(manager);
                analyzer.printComparisonTable();

                std::string reportPath = "output/reports/performance_report.txt";
                std::cout << "\nSaving report to: " << reportPath << "...\n";
                std::string reportBody = "CPU Scheduling Simulator - Analysis Run Successful.\n"
                                         "Check standard output for detailed metrics, Gantt charts, and state transitions.\n"
                                         "Total Processes Analysed: " + std::to_string(manager.getCount()) + "\n";
                if (FileManager::saveReportToFile(reportPath, reportBody)) {
                    std::cout << "   [Success] Report saved.\n";
                }
                break;
            }
            case 7: {
                std::cout << "\nCurrent Workload State:\n";
                manager.displayAll();
                break;
            }
            case 8: {
                manager.clear();
                std::cout << "\n[Success] All processes cleared.\n";
                break;
            }
            case 9: {
                running = false;
                std::cout << "\nExiting Simulator. Goodbye!\n";
                break;
            }
            default:
                std::cout << "Invalid choice. Please select 1-9.\n";
        }
    }

    return 0;
}
