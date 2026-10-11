// This file contains the menu of the program. The real work is done by the
// functions and classes declared in the header files included below.
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

// This function reads a file and prints both final grades of every student.
void showFile(const std::string& fileName) {
    std::vector<Person> students = readStudentsFromFile(fileName);
    sortByName(students);
    printBothFinals(students);
}

// This function returns the names of the five generated test files.
std::vector<std::string> testFileNames() {
    std::vector<std::string> names;
    for (int size : TEST_SIZES)
        names.push_back(testFileName(size));
    return names;
}

// This function shows the menu and does what the user chooses.
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
        std::string fileName;
        std::cout << "File name [all five generated files]: ";
        std::getline(std::cin, fileName);
        const Method method = chooseMethod();
        std::cout << "\n";
        if (fileName.empty())
            runSpeedTest(testFileNames(), method);
        else
            runSpeedTest({fileName}, method);
        return;
    }

    std::vector<Person> students = (choice == 1) ? enterStudents() : generateStudents();
    if (students.empty()) {
        std::cout << "No students entered.\n";
        return;
    }
    sortByName(students);

    const Method method = chooseMethod();
    for (Person& student : students)
        student.calculateFinal(method);
    printFinals(students, method);
}

// This is where the program starts.
// Usage: ./grades                  - menu
//        ./grades <file>           - read a file and print both final grades
//        ./grades generate         - generate the five test data files
//        ./grades test [files...]  - speed test (default: the five generated files)
int main(int argc, char* argv[]) {
    try {
        const std::vector<std::string> args(argv + 1, argv + argc);

        if (args.empty()) {
            runMenu();
        } else if (args[0] == "generate") {
            generateTestFiles(5);
        } else if (args[0] == "test") {
            const std::vector<std::string> files(args.begin() + 1, args.end());
            runSpeedTest(files.empty() ? testFileNames() : files, Method::Average);
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
