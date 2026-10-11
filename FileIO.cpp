// This file contains the file reading, writing and generating functions
// declared in FileIO.h.
#include "FileIO.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include "Errors.h"

std::string finalTitle(Method method) {
    return (method == Method::Average) ? "Final_Point(Aver.)" : "Final_Point(Med.)";
}

std::vector<Person> readStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file)
        throw FileError("cannot open file \"" + fileName + "\"");

    std::string line;
    std::getline(file, line); // skip the header line

    std::vector<Person> students;
    std::istringstream input;
    Person student;
    int skipped = 0;
    while (std::getline(file, line)) {
        // Skip lines that are empty or contain only spaces.
        if (line.find_first_not_of(" \t\r") == std::string::npos)
            continue;
        // The same string stream is reused for every line to save time.
        input.clear();
        input.str(line);
        // The overloaded >> of Person does the reading and checking.
        if (input >> student)
            students.push_back(student);
        else
            ++skipped;
    }

    if (skipped > 0)
        std::cerr << "Skipped " << skipped << " line(s) that could not be read.\n";
    if (students.empty())
        throw DataError("file \"" + fileName + "\" contains no students");
    return students;
}

void writeStudentsToFile(const std::string& fileName,
                         const std::vector<Person>& students, Method method) {
    std::ofstream file(fileName);
    if (!file)
        throw FileError("cannot create file \"" + fileName + "\"");

    file << std::left << std::setw(20) << "Name" << std::setw(20) << "Surname"
         << std::right << std::setw(18) << finalTitle(method) << "\n"
         << std::string(58, '-') << "\n";
    // The overloaded << of Person prints one row.
    for (const Person& student : students)
        file << student << "\n";

    // A full disk shows up here, not when the file is opened.
    if (!file)
        throw FileError("could not write to file \"" + fileName + "\"");
}

void generateStudentFile(const std::string& fileName, int studentCount, int homeworkCount) {
    std::ofstream file(fileName);
    if (!file)
        throw FileError("cannot create file \"" + fileName + "\"");

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> score(1, 10);

    // Header line: Name Surname HW1 HW2 ... Exam
    file << std::left << std::setw(20) << "Name" << std::setw(20) << "Surname" << std::right;
    for (int i = 1; i <= homeworkCount; ++i)
        file << std::setw(6) << ("HW" + std::to_string(i));
    file << std::setw(6) << "Exam" << "\n";

    for (int i = 1; i <= studentCount; ++i) {
        file << std::left << std::setw(20) << ("Name" + std::to_string(i))
             << std::setw(20) << ("Surname" + std::to_string(i)) << std::right;
        // One extra number at the end is the exam result.
        for (int j = 0; j <= homeworkCount; ++j)
            file << std::setw(6) << score(generator);
        file << "\n";
    }

    if (!file)
        throw FileError("could not write to file \"" + fileName + "\"");
}
