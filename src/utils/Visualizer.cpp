#include "utils/Visualizer.h"
#include <iostream>
#include <iomanip>
#include <string>

// ─── Private Helpers ─────────────────────────────────────────────────────────

// Collects ExecutionSteps from the LIFO stack into a forward-chronological array.
// Stack::collectToArray() returns elements top→bottom (LIFO = reverse chronological).
// We reverse in-place to restore chronological order before rendering the chart.
int Visualizer::collectSteps(const Stack<ExecutionStep>& history,
                              ExecutionStep* out, int maxSteps) {
    static ExecutionStep temp[MAX_STEPS];

    // Drain LIFO top→bottom (reverse chronological) via const non-destructive walk
    int count = history.collectToArray(temp, maxSteps);

    // Reverse temp[] → forward chronological order
    int lo = 0, hi = count - 1;
    while (lo < hi) {
        ExecutionStep swp = temp[lo];
        temp[lo] = temp[hi];
        temp[hi] = swp;
        lo++;
        hi--;
    }

    for (int i = 0; i < count && i < maxSteps; ++i) {
        out[i] = temp[i];
    }
    return count;
}

// Renders a single padded Gantt bar cell with label centred inside block width.
void Visualizer::renderBlock(const std::string& label, int duration) {
    int totalWidth = duration * CELL_WIDTH;
    if (totalWidth < static_cast<int>(label.size()) + 2) {
        totalWidth = static_cast<int>(label.size()) + 2;
    }

    int labelLen = static_cast<int>(label.size());
    int padding   = totalWidth - labelLen - 2; // subtract 2 for the two '|' walls (left printed by caller)
    int leftPad   = padding / 2;
    int rightPad  = padding - leftPad;
    if (leftPad  < 0) leftPad  = 0;
    if (rightPad < 0) rightPad = 0;

    std::cout << "|" << std::string(leftPad, ' ') << label << std::string(rightPad, ' ');
}

// ─── Public Interface ─────────────────────────────────────────────────────────

void Visualizer::renderGanttChart(const SchedulerBase& scheduler,
                                   const ProcessManager& processManager,
                                   const std::string& title) {
    // ── Collect execution steps in chronological order ────────────────────
    ExecutionStep steps[MAX_STEPS];
    int stepCount = collectSteps(scheduler.getExecutionHistory(), steps, MAX_STEPS);
    (void)processManager; // reserved for future: per-process colour labels

    if (stepCount == 0) {
        std::cout << "[Visualizer] No execution history to render.\n";
        return;
    }

    std::string chartTitle = title.empty() ? scheduler.getAlgorithmName() : title;

    // ── Header ────────────────────────────────────────────────────────────
    std::cout << "\n";
    std::cout << "==========================================================\n";
    std::cout << "  Gantt Chart: " << chartTitle << "\n";
    std::cout << "==========================================================\n\n";

    // ── Top border ────────────────────────────────────────────────────────
    std::cout << "  ";
    for (int i = 0; i < stepCount; ++i) {
        int duration = steps[i].endTime - steps[i].startTime;
        if (duration < 1) duration = 1;
        int cellW = duration * CELL_WIDTH;
        // Ensure minimum cell width for label
        std::string label = (steps[i].pid == -1) ? "IDLE" : ("P" + std::to_string(steps[i].pid));
        if (cellW < static_cast<int>(label.size()) + 2) {
            cellW = static_cast<int>(label.size()) + 2;
        }
        std::cout << "+" << std::string(cellW, '-');
    }
    std::cout << "+\n";

    // ── Label row ─────────────────────────────────────────────────────────
    std::cout << "  ";
    for (int i = 0; i < stepCount; ++i) {
        int duration = steps[i].endTime - steps[i].startTime;
        if (duration < 1) duration = 1;
        std::string label = (steps[i].pid == -1) ? "IDLE" : ("P" + std::to_string(steps[i].pid));
        renderBlock(label, duration);
    }
    std::cout << "|\n";

    // ── Bottom border ─────────────────────────────────────────────────────
    std::cout << "  ";
    for (int i = 0; i < stepCount; ++i) {
        int duration = steps[i].endTime - steps[i].startTime;
        if (duration < 1) duration = 1;
        int cellW = duration * CELL_WIDTH;
        std::string label = (steps[i].pid == -1) ? "IDLE" : ("P" + std::to_string(steps[i].pid));
        if (cellW < static_cast<int>(label.size()) + 2) {
            cellW = static_cast<int>(label.size()) + 2;
        }
        std::cout << "+" << std::string(cellW, '-');
    }
    std::cout << "+\n";

    // ── Time tick row ─────────────────────────────────────────────────────
    std::cout << "  ";
    for (int i = 0; i < stepCount; ++i) {
        int duration = steps[i].endTime - steps[i].startTime;
        if (duration < 1) duration = 1;
        int cellW = duration * CELL_WIDTH;
        std::string label = (steps[i].pid == -1) ? "IDLE" : ("P" + std::to_string(steps[i].pid));
        if (cellW < static_cast<int>(label.size()) + 2) {
            cellW = static_cast<int>(label.size()) + 2;
        }
        cellW += 1; // account for the '+' character in border rows

        std::string startLabel = std::to_string(steps[i].startTime);
        std::cout << startLabel;
        int remaining = cellW - static_cast<int>(startLabel.size());
        if (remaining < 0) remaining = 0;
        std::cout << std::string(remaining, ' ');
    }
    std::cout << std::to_string(steps[stepCount - 1].endTime) << "\n\n";

    // ── Legend ────────────────────────────────────────────────────────────
    std::cout << "  Execution Timeline:\n";
    for (int i = 0; i < stepCount; ++i) {
        int dur = steps[i].endTime - steps[i].startTime;
        if (steps[i].pid == -1) {
            std::cout << "    [IDLE]  t=" << steps[i].startTime
                      << " -> t=" << steps[i].endTime
                      << "  (" << dur << " unit" << (dur != 1 ? "s" : "") << " idle)\n";
        } else {
            std::cout << "    [P" << steps[i].pid << "]"
                      << (steps[i].pid < 10 ? "    " : "   ")
                      << "t=" << steps[i].startTime
                      << " -> t=" << steps[i].endTime
                      << "  (" << dur << " unit" << (dur != 1 ? "s" : "") << ")\n";
        }
    }

    // ── Performance summary ───────────────────────────────────────────────
    std::cout << "\n  Performance Metrics:\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "    Total CPU Time      : " << scheduler.getTotalCpuTime()      << " units\n";
    std::cout << "    CPU Idle Time       : " << scheduler.getIdleTime()           << " units\n";
    std::cout << "    Avg Turnaround Time : " << scheduler.getAvgTurnaroundTime() << " units\n";
    std::cout << "    Avg Waiting Time    : " << scheduler.getAvgWaitingTime()    << " units\n";
    std::cout << "\n";
}

void Visualizer::renderProcessTable(const ProcessManager& processManager) {
    int count = processManager.getCount();
    if (count == 0) {
        std::cout << "[Visualizer] No processes to display.\n";
        return;
    }

    std::cout << "\n  Process Execution Summary:\n";
    std::cout << "  +-------+----------+--------+----------+--------+--------+--------+\n";
    std::cout << "  |  PID  | Arrival  | Burst  | Priority |   CT   |  TAT   |   WT   |\n";
    std::cout << "  +-------+----------+--------+----------+--------+--------+--------+\n";

    for (int i = 0; i < count; ++i) {
        Process p = processManager.getProcessAt(i);
        std::cout << "  |"
                  << std::setw(6)  << p.id             << " |"
                  << std::setw(9)  << p.arrivalTime     << " |"
                  << std::setw(7)  << p.burstTime       << " |"
                  << std::setw(9)  << p.priority        << " |"
                  << std::setw(7)  << p.completionTime  << " |"
                  << std::setw(7)  << p.turnaroundTime  << " |"
                  << std::setw(7)  << p.waitingTime     << " |\n";
    }

    std::cout << "  +-------+----------+--------+----------+--------+--------+--------+\n\n";
}
