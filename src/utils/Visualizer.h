#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "schedulers/SchedulerBase.h"
#include "core/ProcessManager.h"
#include <string>
#include <iostream>

// ASCII Gantt Chart and Timeline renderer for CPU scheduling simulation results.
// Reads the Stack<ExecutionStep> execution history from any SchedulerBase subclass
// and renders a horizontal ASCII Gantt chart + per-process execution breakdown.
class Visualizer {
public:
    // Renders the ASCII Gantt chart for the given scheduler and workload.
    // Reads executionHistory from scheduler (in reverse Stack order) and displays:
    //   - Gantt bar row (process blocks)
    //   - Time tick row (time markers under each block)
    //   - Per-process execution summary (CT, TAT, WT)
    static void renderGanttChart(const SchedulerBase& scheduler,
                                  const ProcessManager& processManager,
                                  const std::string& title = "");

    // Renders a per-process execution breakdown table showing
    // Arrival, Burst, Completion, TAT, and Waiting Time for each process.
    static void renderProcessTable(const ProcessManager& processManager);

private:
    // Maximum number of execution steps the Visualizer can handle
    static const int MAX_STEPS = 256;

    // Width (characters) to represent each time unit in the Gantt bar
    static const int CELL_WIDTH = 4;

    // Collects execution steps from LIFO Stack into a forward-chronological array.
    // Returns number of steps collected.
    static int collectSteps(const Stack<ExecutionStep>& history,
                            ExecutionStep* out, int maxSteps);

    // Renders a single Gantt bar block cell of given duration width
    static void renderBlock(const std::string& label, int duration);
};

#endif // VISUALIZER_H
