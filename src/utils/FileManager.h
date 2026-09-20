#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "core/ProcessManager.h"
#include <string>

class FileManager {
public:
    // Loads processes from a text file and adds them to ProcessManager.
    // Expected format per line: PID ArrivalTime BurstTime Priority
    // Returns true if successfully loaded, false otherwise.
    static bool loadProcessesFromFile(const std::string& filepath, ProcessManager& pm);

    // Saves a generated report string to a specified filepath.
    // Returns true on success, false on failure.
    static bool saveReportToFile(const std::string& filepath, const std::string& reportContent);
};

#endif // FILE_MANAGER_H
