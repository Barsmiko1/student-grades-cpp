// This file contains the menu and everything around the Person class:
// input, file reading, sorting and printing the result tables.
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Person.h"

// This function keeps asking until the user types a whole number between
// min and max, and returns that number.
int readNumber(const std::string& question, int min, int max) {
    std::string line;
    while (true) {
        std::cout << question;
        // Input has ended (for example Ctrl+D), so stop asking.
        if (!std::getline(std::cin, line))
            return min;
        std::istringstream input(line);
        int value;
        char extra;
        // Valid only if the line is a number, with nothing after it, in range.
        if (input >> value && !(input >> extra) && value >= min && value <= max)
            return value;
        std::cout << "Please enter a number from " << min << " to " << max << ".\n";
    }
}

// This function lets the user type students one per line, with as many
// homework results as they want. An empty line ends the input.
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

// This function lets the user type only names and surnames; the homework
// and exam results of every student are generated randomly.
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

// This function reads students from a text file into the students vector.
// The first line is a header and every other line is
// "Name Surname HW1 ... HWn Exam". It returns false if the file cannot be opened.
bool readStudentsFromFile(const std::string& fileName, std::vector<Person>& students) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Cannot open file \"" << fileName << "\".\n";
        return false;
    }

    std::string line;
    std::getline(file, line); // skip the header line

    int skipped = 0;
    while (std::getline(file, line)) {
        // Skip lines that are empty or contain only spaces.
        if (line.find_first_not_of(" \t\r") == std::string::npos)
            continue;
        std::istringstream input(line);
        Person student;
        if (input >> student)
            students.push_back(student);
        else
            ++skipped;
    }
    if (skipped > 0)
        std::cerr << "Skipped " << skipped << " line(s) that could not be read.\n";
    return true;
}

// This function tells std::sort which of two students comes first:
// ordered by name, and by surname when the names are equal.
bool byName(const Person& a, const Person& b) {
    if (a.name() != b.name())
        return a.name() < b.name();
    return a.surname() < b.surname();
}

// This function prints the table used for file data, with both the average
// and the median based final grade of every student.
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

// This function does the whole file mode: read, sort, print.
// It returns the exit code of the program (0 = success, 1 = error).
int processFile(const std::string& fileName) {
    std::vector<Person> students;
    if (!readStudentsFromFile(fileName, students))
        return 1;
    std::sort(students.begin(), students.end(), byName);
    printBothFinals(students);
    return 0;
}

// This is where the program starts.
// Usage: ./grades            - interactive menu
//        ./grades <file>     - read students from the given file
int main(int argc, char* argv[]) {
    // A file name given on the command line skips the menu.
    if (argc > 1)
        return processFile(argv[1]);

    std::cout << "How do you want to provide the results?\n"
              << "  1 - type them in\n"
              << "  2 - generate them randomly\n"
              << "  3 - read them from a file\n";
    const int source = readNumber("Choice: ", 1, 3);

    if (source == 3) {
        std::string fileName;
        std::cout << "File name [Students.txt]: ";
        std::getline(std::cin, fileName);
        if (fileName.empty())
            fileName = "Students.txt";
        std::cout << "\n";
        return processFile(fileName);
    }

    std::vector<Person> students = (source == 1) ? enterStudents() : generateStudents();
    if (students.empty()) {
        std::cout << "No students entered.\n";
        return 0;
    }
    std::sort(students.begin(), students.end(), byName);

    std::cout << "\nCalculate the final grade using:\n"
              << "  1 - average of homework\n"
              << "  2 - median of homework\n";
    const Method method = (readNumber("Choice: ", 1, 2) == 1) ? Method::Average : Method::Median;
    for (Person& student : students)
        student.calculateFinal(method);

    // The column title shows which method was chosen.
    const std::string title = (method == Method::Average) ? "Final_Point(Aver.)" : "Final_Point(Med.)";
    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << title << "\n"
              << std::string(48, '-') << "\n";
    for (const Person& student : students)
        std::cout << student << "\n";

    return 0;
}
