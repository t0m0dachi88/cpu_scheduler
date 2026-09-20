#include "utils/PerformanceAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

PerformanceAnalyzer::PerformanceAnalyzer(int quantum)
    : resultCount(0), rrQuantum(quantum > 0 ? quantum : DEFAULT_RR_QUANTUM) {}

// ─── Private Helpers ─────────────────────────────────────────────────────────

void PerformanceAnalyzer::recordResult(const SchedulerBase& scheduler) {
    if (resultCount >= MAX_ALGORITHMS) return;

    AlgorithmResult& r = results[resultCount];
    r.algorithmName  = scheduler.getAlgorithmName();
    r.totalCpuTime   = scheduler.getTotalCpuTime();
    r.idleTime       = scheduler.getIdleTime();
    r.avgTurnaround  = scheduler.getAvgTurnaroundTime();
    r.avgWaiting     = scheduler.getAvgWaitingTime();

    if (r.totalCpuTime > 0) {
        r.cpuUtilization = (static_cast<double>(r.totalCpuTime - r.idleTime) /
                            static_cast<double>(r.totalCpuTime)) * 100.0;
    } else {
        r.cpuUtilization = 0.0;
    }

    resultCount++;
}

void PerformanceAnalyzer::printTableDivider() {
    std::cout << "  +---------------------------------+----------+----------+----------+----------+----------+\n";
}

void PerformanceAnalyzer::printTableRow(const AlgorithmResult& r, bool isBest,
                                         double bestTAT, double bestWT, double bestUtil) {
    std::string marker = isBest ? " (*)" : "     ";

    // Truncate algorithm name to 31 chars for column fit
    std::string name = r.algorithmName;
    if (static_cast<int>(name.size()) > 31) {
        name = name.substr(0, 28) + "...";
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  | " << std::left << std::setw(31) << name << std::right << " |";
    std::cout << std::setw(10) << r.totalCpuTime << "|";

    // Highlight best Avg TAT with marker
    std::string tatStr  = std::to_string(r.avgTurnaround).substr(0, 6);
    std::string wtStr   = std::to_string(r.avgWaiting).substr(0, 6);
    std::string utilStr = std::to_string(r.cpuUtilization).substr(0, 6);

    bool isBestTAT  = (std::abs(r.avgTurnaround  - bestTAT)  < 0.001);
    bool isBestWT   = (std::abs(r.avgWaiting      - bestWT)   < 0.001);
    bool isBestUtil = (std::abs(r.cpuUtilization  - bestUtil) < 0.001);

    std::cout << std::setw(9) << r.avgTurnaround << (isBestTAT  ? "*" : " ") << "|";
    std::cout << std::setw(9) << r.avgWaiting    << (isBestWT   ? "*" : " ") << "|";
    std::cout << std::setw(7) << r.idleTime      << "   |";
    std::cout << std::setw(8) << r.cpuUtilization << (isBestUtil ? "*" : " ") << "|\n";
}

// ─── Public Interface ─────────────────────────────────────────────────────────

void PerformanceAnalyzer::analyze(ProcessManager& processManager) {
    resultCount = 0;

    if (processManager.getCount() == 0) {
        std::cout << "[PerformanceAnalyzer] No processes to analyze.\n";
        return;
    }

    // ── Run FCFS ──────────────────────────────────────────────────────────
    {
        FCFS fcfs;
        fcfs.runSimulation(processManager);
        recordResult(fcfs);
        // reset process state for next scheduler
        processManager.resetAll();
    }

    // ── Run Round Robin ───────────────────────────────────────────────────
    {
        RoundRobin rr(rrQuantum);
        rr.runSimulation(processManager);
        recordResult(rr);
        processManager.resetAll();
    }

    // ── Run Priority Scheduler ────────────────────────────────────────────
    {
        PriorityScheduler prioritySched;
        prioritySched.runSimulation(processManager);
        recordResult(prioritySched);
        processManager.resetAll();
    }
}

void PerformanceAnalyzer::printComparisonTable() const {
    if (resultCount == 0) {
        std::cout << "[PerformanceAnalyzer] No results to display. Call analyze() first.\n";
        return;
    }

    // ── Find best (lowest) values for TAT, WT, and highest utilization ────
    double bestTAT  = results[0].avgTurnaround;
    double bestWT   = results[0].avgWaiting;
    double bestUtil = results[0].cpuUtilization;

    for (int i = 1; i < resultCount; ++i) {
        if (results[i].avgTurnaround < bestTAT)  bestTAT  = results[i].avgTurnaround;
        if (results[i].avgWaiting    < bestWT)    bestWT   = results[i].avgWaiting;
        if (results[i].cpuUtilization > bestUtil) bestUtil = results[i].cpuUtilization;
    }

    // ── Table header ──────────────────────────────────────────────────────
    std::cout << "\n";
    std::cout << "  ==========================================================================\n";
    std::cout << "                    ALGORITHM PERFORMANCE COMPARISON\n";
    std::cout << "  ==========================================================================\n";
    std::cout << "  (* = best value in that column)\n\n";

    printTableDivider();
    std::cout << "  | " << std::left  << std::setw(31) << "Algorithm"
              << std::right << " |" << std::setw(10) << "CPU Time"
              << "|" << std::setw(10) << "Avg TAT"
              << "|" << std::setw(10) << "Avg WT"
              << "|" << std::setw(10) << "Idle Time"
              << "|" << std::setw(9)  << "CPU Util" << "|\n";
    printTableDivider();

    // ── One row per algorithm ─────────────────────────────────────────────
    for (int i = 0; i < resultCount; ++i) {
        printTableRow(results[i], false, bestTAT, bestWT, bestUtil);
    }

    printTableDivider();

    // ── Summary footer ────────────────────────────────────────────────────
    std::cout << "\n  Summary:\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "    Best Avg Turnaround Time : " << bestTAT  << " units\n";
    std::cout << "    Best Avg Waiting Time    : " << bestWT   << " units\n";
    std::cout << "    Best CPU Utilization     : " << bestUtil << " %\n\n";
}

const AlgorithmResult& PerformanceAnalyzer::getResult(int index) const {
    if (index < 0 || index >= resultCount) {
        throw std::out_of_range("PerformanceAnalyzer: result index out of range.");
    }
    return results[index];
}
