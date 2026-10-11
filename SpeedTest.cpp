// This file contains the speed analysis functions declared in SpeedTest.h
// (the splitFile template is in the header itself).
#include "SpeedTest.h"

#include <deque>
#include <iomanip>
#include <iostream>
#include <list>

#include "Errors.h"

std::string testFileName(int studentCount) {
    return "generated" + std::to_string(studentCount) + ".txt";
}

void generateTestFiles(int homeworkCount) {
    for (int size : TEST_SIZES) {
        const std::string fileName = testFileName(size);
        Timer timer;
        generateStudentFile(fileName, size, homeworkCount);
        std::cout << "Created " << fileName << " with " << size << " students in "
                  << std::fixed << std::setprecision(3) << timer.elapsed() << " s\n";
    }
}

std::string outputName(const std::string& fileName, const std::string& suffix) {
    const std::size_t dot = fileName.rfind('.');
    if (dot == std::string::npos)
        return fileName + suffix + ".txt";
    return fileName.substr(0, dot) + suffix + fileName.substr(dot);
}

// This function picks the container type that matches the given name.
static Timings splitFileWith(const std::string& container, const std::string& fileName, Method method) {
    if (container == "list")
        return splitFile<std::list<Person>>(fileName, method);
    if (container == "deque")
        return splitFile<std::deque<Person>>(fileName, method);
    return splitFile<std::vector<Person>>(fileName, method);
}

// This structure is one row of the result table.
struct Row {
    std::string container;
    std::string fileName;
    Timings times;
};

void runSpeedTest(const std::vector<std::string>& fileNames, Method method,
                  const std::vector<std::string>& containers) {
    std::vector<Row> rows;

    // Every file is processed with every container, one after another.
    for (const std::string& fileName : fileNames) {
        for (const std::string& container : containers) {
            std::cout << "Processing " << fileName << " with std::" << container << " ..." << std::endl;
            try {
                rows.push_back({container, fileName, splitFileWith(container, fileName, method)});
            } catch (const FileError& error) {
                std::cout << "  File error: " << error.what() << " - skipped.\n";
                break; // the other containers would fail on this file too
            } catch (const DataError& error) {
                std::cout << "  Data error: " << error.what() << " - skipped.\n";
                break;
            }
        }
    }
    if (rows.empty())
        return;

    std::cout << "\nTime in seconds (final grade by "
              << (method == Method::Average ? "average" : "median") << ")\n"
              << std::left << std::setw(24) << "File" << std::setw(8) << "Type" << std::right
              << std::setw(10) << "Students" << std::setw(10) << "Passed" << std::setw(10) << "Failed"
              << std::setw(9) << "Read" << std::setw(9) << "Sort" << std::setw(9) << "Split"
              << std::setw(14) << "Write passed" << std::setw(14) << "Write failed"
              << std::setw(9) << "Total" << "\n"
              << std::string(126, '-') << "\n" << std::fixed << std::setprecision(3);
    for (const Row& row : rows) {
        const Timings& t = row.times;
        std::cout << std::left << std::setw(24) << row.fileName << std::setw(8) << row.container << std::right
                  << std::setw(10) << t.students << std::setw(10) << t.passed << std::setw(10) << t.failed
                  << std::setw(9) << t.read << std::setw(9) << t.sort << std::setw(9) << t.split
                  << std::setw(14) << t.writePassed << std::setw(14) << t.writeFailed
                  << std::setw(9) << t.total << "\n";
    }
}
