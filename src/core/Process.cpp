#include "core/Process.h"
#include <iostream>
#include <iomanip>

std::string processStateToString(ProcessState state) {
    switch (state) {
        case ProcessState::NEW:       return "NEW";
        case ProcessState::READY:     return "READY";
        case ProcessState::RUNNING:   return "RUNNING";
        case ProcessState::COMPLETED: return "COMPLETED";
        default:                      return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, ProcessState state) {
    os << processStateToString(state);
    return os;
}

Process::Process()
    : id(0), arrivalTime(0), burstTime(0), priority(1),
      remainingTime(0), completionTime(0), turnaroundTime(0),
      waitingTime(0), state(ProcessState::NEW) {}

Process::Process(int pid, int arrival, int burst, int prio)
    : id(pid < 1 ? 1 : pid),
      arrivalTime(arrival < 0 ? 0 : arrival),
      burstTime(burst < 1 ? 1 : burst),
      priority(prio < 1 ? 1 : prio),
      remainingTime(burst < 1 ? 1 : burst),
      completionTime(0), turnaroundTime(0),
      waitingTime(0), state(ProcessState::NEW) {}

void Process::calculateMetrics() {
    turnaroundTime = completionTime - arrivalTime;
    waitingTime = turnaroundTime - burstTime;
    if (waitingTime < 0) {
        waitingTime = 0;
    }
}

void Process::reset() {
    remainingTime = burstTime;
    completionTime = 0;
    turnaroundTime = 0;
    waitingTime = 0;
    state = ProcessState::NEW;
}

void Process::printProcessDetails() const {
    std::cout << "| " << std::setw(5) << id
              << " | " << std::setw(8) << arrivalTime
              << " | " << std::setw(6) << burstTime
              << " | " << std::setw(8) << priority
              << " | " << std::setw(6) << completionTime
              << " | " << std::setw(6) << turnaroundTime
              << " | " << std::setw(6) << waitingTime
              << " | " << std::setw(10) << processStateToString(state)
              << " |\n";
}
