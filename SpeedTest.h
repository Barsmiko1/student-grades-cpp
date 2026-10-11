// This file declares the speed analysis: reading a data file, sorting the
// students, dividing them into passed and failed, writing both groups to
// new files - and measuring how long every step takes.
#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <string>
#include <vector>

#include "Person.h"

// The sizes of the generated test files (number of students in each).
const std::vector<int> TEST_SIZES = {1000, 10000, 100000, 1000000, 10000000};

// This function returns the name of the generated file with that many students.
std::string testFileName(int studentCount);

// This function creates the five test data files.
void generateTestFiles(int homeworkCount);

// This structure holds the measured times (in seconds) for one file.
struct Timings {
    std::size_t students = 0;
    std::size_t passed = 0;
    std::size_t failed = 0;
    double read = 0;
    double sort = 0;
    double split = 0;
    double writePassed = 0;
    double writeFailed = 0;
    double total = 0;
};

// This function processes one file (read, sort, split, write two files)
// and returns the time every step took. For "data.txt" the results go to
// "data_passed.txt" and "data_failed.txt".
Timings splitFile(const std::string& fileName, Method method);

// This function runs splitFile() on every given file and prints a table
// with the measured times. A file that cannot be processed is reported
// and the test continues with the next one.
void runSpeedTest(const std::vector<std::string>& fileNames, Method method);

#endif
