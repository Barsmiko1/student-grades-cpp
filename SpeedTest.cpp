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

static Timings splitFileWith(const std::string& container, const std::string& fileName,
                             Method method, Strategy strategy, bool noLimit) {
    if (container == "list")
        return splitFile<std::list<Person>>(fileName, method, strategy, noLimit);
    if (container == "deque")
        return splitFile<std::deque<Person>>(fileName, method, strategy, noLimit);
    return splitFile<std::vector<Person>>(fileName, method, strategy, noLimit);
}

struct Row {
    std::string fileName;
    std::string container;
    Strategy strategy;
    Timings times;
};

void runSpeedTest(const std::vector<std::string>& fileNames, Method method,
                  const std::vector<std::string>& containers,
                  const std::vector<Strategy>& strategies, bool noLimit) {
    std::vector<Row> rows;

    for (const std::string& fileName : fileNames) {
        bool fileFailed = false;
        for (const std::string& container : containers) {
            for (Strategy strategy : strategies) {
                std::cout << "Processing " << fileName << " with std::" << container << ", "
                          << strategyTitle(strategy) << " ..." << std::endl;
                try {
                    rows.push_back({fileName, container, strategy,
                                    splitFileWith(container, fileName, method, strategy, noLimit)});
                } catch (const FileError& error) {
                    std::cout << "  File error: " << error.what() << " - skipped.\n";
                    fileFailed = true; // the other runs would fail on this file too
                } catch (const DataError& error) {
                    std::cout << "  Data error: " << error.what() << " - skipped.\n";
                    fileFailed = true;
                }
                if (fileFailed)
                    break;
            }
            if (fileFailed)
                break;
        }
    }
    if (rows.empty())
        return;

    std::cout << "\nTime in seconds (final grade by "
              << (method == Method::Average ? "average" : "median") << ")\n"
              << std::left << std::setw(24) << "File" << std::setw(8) << "Type"
              << std::setw(11) << "Strategy" << std::right
              << std::setw(10) << "Students" << std::setw(10) << "Passed" << std::setw(10) << "Failed"
              << std::setw(9) << "Read" << std::setw(9) << "Split" << std::setw(9) << "Sort"
              << std::setw(14) << "Write passed" << std::setw(14) << "Write failed"
              << std::setw(9) << "Total" << "\n"
              << std::string(137, '-') << "\n" << std::fixed << std::setprecision(3);
    for (const Row& row : rows) {
        const Timings& t = row.times;
        std::cout << std::left << std::setw(24) << row.fileName << std::setw(8) << row.container
                  << std::setw(11) << STRATEGIES[static_cast<int>(row.strategy)] << std::right
                  << std::setw(10) << t.students;
        if (t.skipped) {
            std::cout << "   skipped: erase() in a loop would take hours (use --full to force)\n";
            continue;
        }
        std::cout << std::setw(10) << t.passed << std::setw(10) << t.failed
                  << std::setw(9) << t.read << std::setw(9) << t.split << std::setw(9) << t.sort
                  << std::setw(14) << t.writePassed << std::setw(14) << t.writeFailed
                  << std::setw(9) << t.total << "\n";
    }
}
