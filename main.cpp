#include <algorithm>
#include <iostream>
#include <new>
#include <string>
#include <vector>

#include "Console.h"
#include "Errors.h"
#include "FileIO.h"
#include "Person.h"
#include "SpeedTest.h"
#include "Students.h"

// In menu order, matching STRATEGIES.
const std::vector<Strategy> ALL_STRATEGIES = {Strategy::Copy, Strategy::Erase,
                                              Strategy::CopyAlgorithms, Strategy::Partition};

void showFile(const std::string& fileName) {
    std::vector<Person> students = readStudentsFromFile<std::vector<Person>>(fileName);
    sortByName(students);
    printBothFinals(students);
}

std::vector<std::string> testFileNames() {
    std::vector<std::string> names(TEST_SIZES.size());
    std::transform(TEST_SIZES.begin(), TEST_SIZES.end(), names.begin(), testFileName);
    return names;
}

void runSpeedTestMenu() {
    std::string fileName;
    std::cout << "File name [all five generated files]: ";
    std::getline(std::cin, fileName);

    std::cout << "\nContainer to use:\n"
              << "  1 - std::vector\n"
              << "  2 - std::list\n"
              << "  3 - std::deque\n"
              << "  4 - all three, to compare them\n";
    const int container = readNumber("Choice: ", 1, 4);
    const std::vector<std::string> containers =
        (container == 4) ? CONTAINERS : std::vector<std::string>{CONTAINERS[container - 1]};

    const int all = static_cast<int>(ALL_STRATEGIES.size()) + 1;
    std::cout << "\nHow to divide the students into passed and failed:\n";
    for (int i = 1; i < all; ++i)
        std::cout << "  " << i << " - " << strategyTitle(ALL_STRATEGIES[i - 1]) << "\n";
    std::cout << "  " << all << " - all of them, to compare them\n";
    const int strategy = readNumber("Choice: ", 1, all);
    const std::vector<Strategy> strategies =
        (strategy == all) ? ALL_STRATEGIES : std::vector<Strategy>{ALL_STRATEGIES[strategy - 1]};

    const Method method = chooseMethod();
    std::cout << "\n";
    runSpeedTest(fileName.empty() ? testFileNames() : std::vector<std::string>{fileName},
                 method, containers, strategies, false);
}

void runMenu() {
    std::cout << "What do you want to do?\n"
              << "  1 - type the results in\n"
              << "  2 - generate the results randomly\n"
              << "  3 - read the results from a file\n"
              << "  4 - generate the five test data files\n"
              << "  5 - split a file into passed/failed and measure the speed\n";
    const int choice = readNumber("Choice: ", 1, 5);

    if (choice == 3) {
        std::string fileName;
        std::cout << "File name [Students.txt]: ";
        std::getline(std::cin, fileName);
        if (fileName.empty())
            fileName = "Students.txt";
        std::cout << "\n";
        showFile(fileName);
        return;
    }

    if (choice == 4) {
        generateTestFiles(readNumber("How many homework results should each student get? ", 1, 20));
        return;
    }

    if (choice == 5) {
        runSpeedTestMenu();
        return;
    }

    std::vector<Person> students = (choice == 1) ? enterStudents() : generateStudents();
    if (students.empty()) {
        std::cout << "No students entered.\n";
        return;
    }
    sortByName(students);

    const Method method = chooseMethod();
    std::for_each(students.begin(), students.end(),
                  [method](Person& student) { student.calculateFinal(method); });
    printFinals(students, method);
}

// Words after "test": container names, strategy names, "--full"; the rest are files.
void runSpeedTestArgs(const std::vector<std::string>& args) {
    std::vector<std::string> containers, files;
    std::vector<Strategy> strategies;
    bool noLimit = false;

    for (const std::string& arg : args) {
        const auto container = std::find(CONTAINERS.begin(), CONTAINERS.end(), arg);
        const auto strategy = std::find(STRATEGIES.begin(), STRATEGIES.end(), arg);
        if (arg == "--full")
            noLimit = true;
        else if (container != CONTAINERS.end())
            containers.push_back(arg);
        else if (strategy != STRATEGIES.end())
            strategies.push_back(ALL_STRATEGIES[strategy - STRATEGIES.begin()]);
        else
            files.push_back(arg);
    }

    runSpeedTest(files.empty() ? testFileNames() : files, Method::Average,
                 containers.empty() ? CONTAINERS : containers,
                 strategies.empty() ? ALL_STRATEGIES : strategies, noLimit);
}

// ./grades                 menu
// ./grades <file>          print both final grades of every student
// ./grades generate        create the five test data files
// ./grades test [vector|list|deque]... [strategy]... [--full] [files...]
int main(int argc, char* argv[]) {
    try {
        const std::vector<std::string> args(argv + 1, argv + argc);

        if (args.empty()) {
            runMenu();
        } else if (args[0] == "generate") {
            generateTestFiles(5);
        } else if (args[0] == "test") {
            runSpeedTestArgs(std::vector<std::string>(args.begin() + 1, args.end()));
        } else {
            showFile(args[0]);
        }
    } catch (const FileError& error) {
        std::cerr << "File error: " << error.what() << ".\n";
        return 1;
    } catch (const DataError& error) {
        std::cerr << "Data error: " << error.what() << ".\n";
        return 1;
    } catch (const std::bad_alloc&) {
        std::cerr << "Not enough memory to hold that many students.\n";
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "Unexpected error: " << error.what() << ".\n";
        return 1;
    }
    return 0;
}
