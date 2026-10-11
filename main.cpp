#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Person.h"

// Keeps asking until the user types a whole number between min and max.
int readNumber(const std::string& question, int min, int max) {
    std::string line;
    while (true) {
        std::cout << question;
        if (!std::getline(std::cin, line))
            return min;
        std::istringstream input(line);
        int value;
        char extra;
        if (input >> value && !(input >> extra) && value >= min && value <= max)
            return value;
        std::cout << "Please enter a number from " << min << " to " << max << ".\n";
    }
}

// The user types one student per line and decides how many homework results
// to give; an empty line ends the input.
std::vector<Person> enterStudents() {
    std::cout << "\nEnter one student per line: Name Surname HW1 HW2 ... Exam\n"
              << "Type as many homework results as you have - the last number is the exam.\n"
              << "Press Enter on an empty line when you are done.\n";

    std::vector<Person> students;
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line) && !line.empty()) {
        std::istringstream input(line);
        Person student;
        if (input >> student)
            students.push_back(student);
        else
            std::cout << "Could not read that line (results must be 0-10), try again.\n";
    }
    return students;
}

// The user types only the names; homework and exam results are random.
std::vector<Person> generateStudents() {
    const int homeworkCount = readNumber("Number of homework assignments to generate: ", 1, 100);
    std::cout << "\nEnter one student per line: Name Surname\n"
              << "Press Enter on an empty line when you are done.\n";

    std::vector<Person> students;
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line) && !line.empty()) {
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

int main() {
    std::cout << "How do you want to provide the results?\n"
              << "  1 - type them in\n"
              << "  2 - generate them randomly\n";
    const int source = readNumber("Choice: ", 1, 2);

    std::vector<Person> students = (source == 1) ? enterStudents() : generateStudents();
    if (students.empty()) {
        std::cout << "No students entered.\n";
        return 0;
    }

    std::cout << "\nCalculate the final grade using:\n"
              << "  1 - average of homework\n"
              << "  2 - median of homework\n";
    const Method method = (readNumber("Choice: ", 1, 2) == 1) ? Method::Average : Method::Median;
    for (Person& student : students)
        student.calculateFinal(method);

    const std::string title = (method == Method::Average) ? "Final_Point(Aver.)" : "Final_Point(Med.)";
    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << title << "\n"
              << std::string(48, '-') << "\n";
    for (const Person& student : students)
        std::cout << student << "\n";

    return 0;
}
