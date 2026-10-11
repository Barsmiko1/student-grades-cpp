// This file declares the speed analysis: reading a data file, sorting the
// students, dividing them into passed and failed, writing both groups to
// new files - and measuring how long every step takes. The same steps can
// be run with std::vector, std::list or std::deque to compare them.
#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <string>
#include <vector>

#include "FileIO.h"
#include "Person.h"
#include "Students.h"
#include "Timer.h"

// The sizes of the generated test files (number of students in each).
const std::vector<int> TEST_SIZES = {1000, 10000, 100000, 1000000, 10000000};

// The containers the speed test can use.
const std::vector<std::string> CONTAINERS = {"vector", "list", "deque"};

// This function returns the name of the generated file with that many students.
std::string testFileName(int studentCount);

// This function creates the five test data files.
void generateTestFiles(int homeworkCount);

// This function turns "data.txt" into "data_passed.txt" (suffix = "_passed").
std::string outputName(const std::string& fileName, const std::string& suffix);

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
// using the given container type, for example
// splitFile<std::deque<Person>>(name, method), and returns the time every
// step took. For "data.txt" the results go to "data_passed.txt" and
// "data_failed.txt".
template <typename Container>
Timings splitFile(const std::string& fileName, Method method) {
    Timings times;
    Timer total;
    Timer step;

    // Step 1: read the file and calculate every final grade.
    Container students = readStudentsFromFile<Container>(fileName);
    for (Person& student : students)
        student.calculateFinal(method);
    times.read = step.elapsed();
    times.students = students.size();

    // Step 2: sort the students, best final grade first.
    step.reset();
    sortByFinal(students);
    times.sort = step.elapsed();

    // Step 3: divide the students into two new containers.
    step.reset();
    Container passed, failed;
    splitStudents(students, passed, failed);
    times.split = step.elapsed();
    times.passed = passed.size();
    times.failed = failed.size();

    // The full list is not needed any more: swapping it with an empty
    // container gives its memory back (this works for every container type).
    Container().swap(students);

    // Step 4: write each group to its own file.
    step.reset();
    writeStudentsToFile(outputName(fileName, "_passed"), passed, method);
    times.writePassed = step.elapsed();

    step.reset();
    writeStudentsToFile(outputName(fileName, "_failed"), failed, method);
    times.writeFailed = step.elapsed();

    times.total = total.elapsed();
    return times;
}

// This function runs splitFile() on every given file with every given
// container ("vector", "list", "deque") and prints a table with the
// measured times. A file that cannot be processed is reported and the test
// continues with the next one.
void runSpeedTest(const std::vector<std::string>& fileNames, Method method,
                  const std::vector<std::string>& containers);

#endif
