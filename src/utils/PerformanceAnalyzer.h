#ifndef PERFORMANCE_ANALYZER_H
#define PERFORMANCE_ANALYZER_H

#include "schedulers/FCFS.h"
#include "schedulers/RoundRobin.h"
#include "schedulers/PriorityScheduler.h"
#include "core/ProcessManager.h"
#include <string>
#include <iostream>

// Holds aggregated performance metrics for one scheduling algorithm run
struct AlgorithmResult {
    std::string algorithmName;   // Display name of the algorithm
    int         totalCpuTime;    // Total elapsed simulation time
    int         idleTime;        // Total CPU idle time
    double      avgTurnaround;   // Average Turnaround Time across all processes
    double      avgWaiting;      // Average Waiting Time across all processes
    double      cpuUtilization;  // CPU utilization % = (totalCpuTime - idleTime) / totalCpuTime * 100

    AlgorithmResult()
        : algorithmName(""), totalCpuTime(0), idleTime(0),
          avgTurnaround(0.0), avgWaiting(0.0), cpuUtilization(0.0) {}
};

// Runs all registered scheduling algorithms on the same workload and renders
// a side-by-side comparative performance table. No STL containers used.
class PerformanceAnalyzer {
public:
    // Maximum number of algorithms the analyzer tracks per run
    static const int MAX_ALGORITHMS = 8;

    // Default RR time quantum used when running comparative analysis
    static const int DEFAULT_RR_QUANTUM = 2;

    // Constructs analyzer with a configurable RR time quantum
    explicit PerformanceAnalyzer(int rrQuantum = DEFAULT_RR_QUANTUM);

    // Runs FCFS, Round Robin, and Priority Scheduler on the given workload.
    // Results are stored internally in results[].
    // Note: The original ProcessManager workload is restored after each run.
    void analyze(ProcessManager& processManager);

    // Renders the comparative algorithm performance table to stdout.
    // Must be called after analyze().
    void printComparisonTable() const;

    // Returns number of algorithm results collected
    int getResultCount() const { return resultCount; }

    // Returns a const reference to a specific result by index
    const AlgorithmResult& getResult(int index) const;

private:
    AlgorithmResult results[MAX_ALGORITHMS]; // Collected per-algorithm results
    int resultCount;                         // Number of valid results stored
    int rrQuantum;                           // Configured RR time quantum

    // Records metrics from a scheduler run into results[]
    void recordResult(const SchedulerBase& scheduler);

    // Renders a horizontal divider row for the table
    static void printTableDivider();

    // Renders a formatted table row with one algorithm's metrics
    static void printTableRow(const AlgorithmResult& r, bool isBest,
                               double bestTAT, double bestWT, double bestUtil);
};

#endif // PERFORMANCE_ANALYZER_H
