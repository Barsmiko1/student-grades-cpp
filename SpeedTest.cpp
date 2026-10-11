// This file contains the speed analysis functions declared in SpeedTest.h.
#include "SpeedTest.h"

#include <iomanip>
#include <iostream>

#include "Errors.h"
#include "FileIO.h"
#include "Students.h"
#include "Timer.h"

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

// This function turns "data.txt" into "data_passed.txt" (suffix = "_passed").
static std::string outputName(const std::string& fileName, const std::string& suffix) {
    const std::size_t dot = fileName.rfind('.');
    if (dot == std::string::npos)
        return fileName + suffix + ".txt";
    return fileName.substr(0, dot) + suffix + fileName.substr(dot);
}

Timings splitFile(const std::string& fileName, Method method) {
    Timings times;
    Timer total;
    Timer step;

    // Step 1: read the file and calculate every final grade.
    std::vector<Person> students = readStudentsFromFile(fileName);
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
    std::vector<Person> passed, failed;
    splitStudents(students, passed, failed);
    times.split = step.elapsed();
    times.passed = passed.size();
    times.failed = failed.size();

    // The full list is not needed any more, so its memory is given back.
    students.clear();
    students.shrink_to_fit();

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

void runSpeedTest(const std::vector<std::string>& fileNames, Method method) {
    std::vector<std::string> done;
    std::vector<Timings> results;

    for (const std::string& fileName : fileNames) {
        std::cout << "Processing " << fileName << " ..." << std::endl;
        try {
            results.push_back(splitFile(fileName, method));
            done.push_back(fileName);
        } catch (const FileError& error) {
            std::cout << "  File error: " << error.what() << " - skipped.\n";
        } catch (const DataError& error) {
            std::cout << "  Data error: " << error.what() << " - skipped.\n";
        }
    }
    if (results.empty())
        return;

    std::cout << "\nTime in seconds (final grade by "
              << (method == Method::Average ? "average" : "median") << ")\n"
              << std::left << std::setw(24) << "File" << std::right
              << std::setw(10) << "Students" << std::setw(10) << "Passed" << std::setw(10) << "Failed"
              << std::setw(9) << "Read" << std::setw(9) << "Sort" << std::setw(9) << "Split"
              << std::setw(14) << "Write passed" << std::setw(14) << "Write failed"
              << std::setw(9) << "Total" << "\n"
              << std::string(118, '-') << "\n" << std::fixed << std::setprecision(3);
    for (std::size_t i = 0; i < results.size(); ++i) {
        const Timings& t = results[i];
        std::cout << std::left << std::setw(24) << done[i] << std::right
                  << std::setw(10) << t.students << std::setw(10) << t.passed << std::setw(10) << t.failed
                  << std::setw(9) << t.read << std::setw(9) << t.sort << std::setw(9) << t.split
                  << std::setw(14) << t.writePassed << std::setw(14) << t.writeFailed
                  << std::setw(9) << t.total << "\n";
    }
}
