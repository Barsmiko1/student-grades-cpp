// This file contains the keyboard input and screen output functions
// declared in Console.h.
#include "Console.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "FileIO.h"

int readNumber(const std::string& question, int min, int max) {
    std::string line;
    while (true) {
        std::cout << question;
        // Input has ended (for example Ctrl+D), so stop asking.
        if (!std::getline(std::cin, line))
            return min;

        try {
            // std::stoi throws if the text is not a number or is too big.
            std::size_t used = 0;
            const int value = std::stoi(line, &used);
            if (line.find_first_not_of(" \t\r", used) != std::string::npos)
                throw std::invalid_argument("text after the number");
            if (value < min || value > max)
                throw std::out_of_range("number outside the allowed range");
            return value;
        } catch (const std::invalid_argument&) {
            std::cout << "That is not a whole number.\n";
        } catch (const std::out_of_range&) {
            std::cout << "Please enter a number from " << min << " to " << max << ".\n";
        }
    }
}

Method chooseMethod() {
    std::cout << "\nCalculate the final grade using:\n"
              << "  1 - average of homework\n"
              << "  2 - median of homework\n";
    return (readNumber("Choice: ", 1, 2) == 1) ? Method::Average : Method::Median;
}

std::vector<Person> enterStudents() {
    std::cout << "\nEnter one student per line: Name Surname HW1 HW2 ... Exam\n"
              << "Type as many homework results as you have - the last number is the exam.\n"
              << "Add as many students as you like, then press Enter on an empty line to finish.\n";

    std::vector<Person> students;
    std::string line;
    while (std::cout << "Student " << students.size() + 1 << "> " && std::getline(std::cin, line) && !line.empty()) {
        std::istringstream input(line);
        Person student;
        // The overloaded >> of Person does the reading and checking.
        if (input >> student)
            students.push_back(student);
        else
            std::cout << "Could not read that line (results must be 0-10), try again.\n";
    }
    return students;
}

std::vector<Person> generateStudents() {
    const int homeworkCount = readNumber("How many homework results should each student get? ", 1, 100);
    std::cout << "\nNow enter the students, one per line: Name Surname\n"
              << "Add as many students as you like, then press Enter on an empty line to finish.\n";

    std::vector<Person> students;
    std::string line;
    while (std::cout << "Student " << students.size() + 1 << "> " && std::getline(std::cin, line) && !line.empty()) {
        std::istringstream input(line);
        std::string name, surname;
        if (input >> name >> surname) {
            Person student(name, surname);
            student.generateRandomScores(homeworkCount);
            students.push_back(student);
        } else {
            std::cout << "Please type both a name and a surname.\n";
        }
    }
    return students;
}

void printFinals(const std::vector<Person>& students, Method method) {
    // The column title shows which method was chosen.
    std::cout << "\n" << std::left << std::setw(20) << "Name"
              << std::setw(20) << "Surname"
              << std::right << std::setw(18) << finalTitle(method) << "\n"
              << std::string(58, '-') << "\n";
    for (const Person& student : students)
        std::cout << student << "\n";
}

void printBothFinals(const std::vector<Person>& students) {
    std::cout << std::fixed << std::setprecision(2)
              << std::left << std::setw(20) << "Name" << std::setw(20) << "Surname"
              << std::right << std::setw(13) << "Final (Avg.)" << " | "
              << std::setw(12) << "Final (Med.)" << "\n"
              << std::string(68, '-') << "\n";
    for (const Person& student : students)
        std::cout << std::left << std::setw(20) << student.name()
                  << std::setw(20) << student.surname()
                  << std::right << std::setw(13) << student.finalGrade(Method::Average) << " | "
                  << std::setw(12) << student.finalGrade(Method::Median) << "\n";
}
