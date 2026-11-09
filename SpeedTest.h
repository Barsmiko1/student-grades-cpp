// Speed analysis: read a file, divide the students into passed and failed,
// sort both groups and write them to two new files, timing every step.
#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <list>
#include <string>
#include <type_traits>
#include <vector>

#include "FileIO.h"
#include "Person.h"
#include "Students.h"
#include "Timer.h"

const std::vector<int> TEST_SIZES = {1000, 10000, 100000, 1000000, 10000000};
const std::vector<std::string> CONTAINERS = {"vector", "list", "deque"};

// Above this size the erase() loop on a vector or deque would run for hours,
// so it is skipped unless "--full" is given.
const std::size_t SLOW_ERASE_LIMIT = 100000;

std::string testFileName(int studentCount);
void generateTestFiles(int homeworkCount);

// "data.txt" + "_passed" -> "data_passed.txt"
std::string outputName(const std::string& fileName, const std::string& suffix);

// The most memory (MB) this process has used so far.
double peakMemoryMB();

// Times in seconds for one file.
struct Timings {
    bool skipped = false;
    std::size_t students = 0;
    std::size_t passed = 0;
    std::size_t failed = 0;
    double read = 0;
    double split = 0;
    double sort = 0;
    double writePassed = 0;
    double writeFailed = 0;
    double total = 0;
};

// Usage: splitFile<std::deque<Person>>(fileName, method, Strategy::Erase, false)
template <typename Container>
Timings splitFile(const std::string& fileName, Method method, Strategy strategy, bool noLimit) {
    Timings times;
    Timer total;
    Timer step;

    Container students = readStudentsFromFile<Container>(fileName);
    std::for_each(students.begin(), students.end(),
                  [method](Person& student) { student.calculateFinal(method); });
    times.read = step.elapsed();
    times.students = students.size();

    const bool isList = std::is_same<Container, std::list<Person>>::value;
    if (strategy == Strategy::Erase && !isList && !noLimit && students.size() > SLOW_ERASE_LIMIT) {
        times.skipped = true;
        return times;
    }

    // The split runs before sorting: on a sorted container the failed
    // students would already sit together at the end.
    step.reset();
    Container passed, failed;
    switch (strategy) {
    case Strategy::Copy:           splitCopy(students, passed, failed); break;
    case Strategy::CopyAlgorithms: splitCopyAlgorithms(students, passed, failed); break;
    case Strategy::Erase:          splitErase(students, failed); break;
    case Strategy::Partition:      splitPartition(students, failed); break;
    }
    times.split = step.elapsed();

    // With Strategy 2 the base container itself holds the passed students.
    const bool keepsBase = (strategy == Strategy::Erase || strategy == Strategy::Partition);
    Container& passedGroup = keepsBase ? students : passed;
    times.passed = passedGroup.size();
    times.failed = failed.size();

    step.reset();
    sortByFinal(passedGroup);
    sortByFinal(failed);
    times.sort = step.elapsed();

    step.reset();
    writeStudentsToFile(outputName(fileName, "_passed"), passedGroup, method);
    times.writePassed = step.elapsed();

    step.reset();
    writeStudentsToFile(outputName(fileName, "_failed"), failed, method);
    times.writeFailed = step.elapsed();

    times.total = total.elapsed();
    return times;
}

// Runs every file with every container and strategy and prints a table.
// A file that cannot be processed is reported and skipped.
void runSpeedTest(const std::vector<std::string>& fileNames, Method method,
                  const std::vector<std::string>& containers,
                  const std::vector<Strategy>& strategies, bool noLimit);

#endif
