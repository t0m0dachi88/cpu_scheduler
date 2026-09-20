#include "utils/FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool FileManager::loadProcessesFromFile(const std::string& filepath, ProcessManager& pm) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[FileManager] Error: Cannot open file for reading: " << filepath << "\n";
        return false;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        
        // Skip empty lines or comments (starting with #)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        int pid, arrivalTime, burstTime, priority;
        
        // Try to read exactly 4 integers
        if (ss >> pid >> arrivalTime >> burstTime >> priority) {
            pm.addProcess(pid, arrivalTime, burstTime, priority);
        } else {
            std::cerr << "[FileManager] Warning: Invalid format on line " << lineNum 
                      << " of " << filepath << "\n";
        }
    }

    file.close();
    return true;
}

bool FileManager::saveReportToFile(const std::string& filepath, const std::string& reportContent) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[FileManager] Error: Cannot open file for writing: " << filepath << "\n";
        return false;
    }

    file << reportContent;
    file.close();
    return true;
}
