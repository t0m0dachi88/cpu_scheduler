#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "utils/FileManager.h"
#include "core/ProcessManager.h"

#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL]: " << message << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

#define ASSERT_EQUAL(actual, expected, message) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL]: " << message << " (Expected: " << (expected) << ", Got: " << (actual) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false; \
        } \
    } while (0)

// Helper function to create a temporary test file
void createTempFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    file << content;
    file.close();
}

// TC-001: Successfully load valid processes from a file
bool testLoadValidProcesses() {
    std::string filepath = "test_valid_processes.txt";
    std::string content = 
        "# This is a comment\n"
        "1 0 5 2\n"
        "2 1 3 1\n"
        "\n"
        "3 2 8 3\n";
    
    createTempFile(filepath, content);

    ProcessManager pm;
    bool success = FileManager::loadProcessesFromFile(filepath, pm);
    
    ASSERT_TRUE(success, "Loading from valid file should return true");
    ASSERT_EQUAL(pm.getCount(), 3, "Should have loaded exactly 3 processes");

    Process p1 = pm.getProcessAt(0);
    ASSERT_EQUAL(p1.id, 1, "P1 id");
    ASSERT_EQUAL(p1.arrivalTime, 0, "P1 AT");
    ASSERT_EQUAL(p1.burstTime, 5, "P1 BT");
    ASSERT_EQUAL(p1.priority, 2, "P1 Prio");

    Process p3 = pm.getProcessAt(2);
    ASSERT_EQUAL(p3.id, 3, "P3 id");
    ASSERT_EQUAL(p3.burstTime, 8, "P3 BT");

    std::remove(filepath.c_str());
    return true;
}

// TC-002: Handle invalid formats gracefully
bool testLoadInvalidFormat() {
    std::string filepath = "test_invalid_processes.txt";
    std::string content = 
        "1 0 5 2\n"
        "bad 1 3 1\n" // invalid format
        "3 2 8\n";    // missing priority (invalid)
    
    createTempFile(filepath, content);

    ProcessManager pm;
    bool success = FileManager::loadProcessesFromFile(filepath, pm);
    
    ASSERT_TRUE(success, "Loading from file should return true even with skipped lines");
    ASSERT_EQUAL(pm.getCount(), 1, "Should have loaded only 1 valid process");
    ASSERT_EQUAL(pm.getProcessAt(0).id, 1, "The valid process should be P1");

    std::remove(filepath.c_str());
    return true;
}

// TC-003: Save report and read it back
bool testSaveReport() {
    std::string filepath = "test_report_output.txt";
    std::string reportData = "PERFORMANCE ANALYZER REPORT\nLine 2 data\nEnd.";

    bool success = FileManager::saveReportToFile(filepath, reportData);
    ASSERT_TRUE(success, "Saving report should return true");

    std::ifstream inFile(filepath);
    ASSERT_TRUE(inFile.is_open(), "Report file should exist");

    std::string content((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
    inFile.close();

    ASSERT_EQUAL(content, reportData, "Report content should match saved data");

    std::remove(filepath.c_str());
    return true;
}

// TC-004: Handle non-existent file reading
bool testReadNonExistentFile() {
    ProcessManager pm;
    bool success = FileManager::loadProcessesFromFile("does_not_exist_xyz.txt", pm);
    ASSERT_TRUE(!success, "Loading from non-existent file should return false");
    ASSERT_EQUAL(pm.getCount(), 0, "No processes should be loaded");
    return true;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     RUNNING FILE MANAGER TESTS         \n";
    std::cout << "========================================\n\n";

    int passed = 0;
    int total = 4;

    auto runTest = [&](const std::string& name, bool (*testFunc)()) {
        std::cout << "[TEST]: " << name << " ... ";
        if (testFunc()) {
            std::cout << "PASSED\n";
            passed++;
        } else {
            std::cout << "FAILED\n";
        }
    };

    runTest("Load Valid Processes (Comments/Blank Lines)", testLoadValidProcesses);
    runTest("Skip Invalid Formatted Lines", testLoadInvalidFormat);
    runTest("Save Report to File", testSaveReport);
    runTest("Handle Non-existent File", testReadNonExistentFile);

    std::cout << "\n----------------------------------------\n";
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
    std::cout << "----------------------------------------\n";

    return (passed == total) ? 0 : 1;
}
