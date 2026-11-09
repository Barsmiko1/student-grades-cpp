// Reading, writing and generating student data files. Reading and writing
// are templates, so they work for std::vector, std::list and std::deque.
#ifndef FILEIO_H
#define FILEIO_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "Errors.h"
#include "Person.h"

inline bool isNotSpace(char c) {
    return !std::isspace(static_cast<unsigned char>(c));
}

std::string finalTitle(Method method);

// Creates a file with students Name1 Surname1, Name2 Surname2, ... and
// random results from 1 to 10.
void generateStudentFile(const std::string& fileName, int studentCount, int homeworkCount);

// The first line is a header, every other line is "Name Surname HW1 ... HWn Exam".
// Usage: readStudentsFromFile<std::list<Person>>(fileName)
template <typename Container>
Container readStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file)
        throw FileError("cannot open file \"" + fileName + "\"");

    std::string line;
    std::getline(file, line); // header

    Container students;
    std::istringstream input; // reused for every line to save time
    Person student;
    int skipped = 0;
    while (std::getline(file, line)) {
        if (std::find_if(line.begin(), line.end(), isNotSpace) == line.end())
            continue; // blank line
        input.clear();
        input.str(line);
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

template <typename Container>
void writeStudentsToFile(const std::string& fileName, const Container& students, Method method) {
    std::ofstream file(fileName);
    if (!file)
        throw FileError("cannot create file \"" + fileName + "\"");

    file << std::left << std::setw(20) << "Name" << std::setw(20) << "Surname"
         << std::right << std::setw(18) << finalTitle(method) << "\n"
         << std::string(58, '-') << "\n";
    std::copy(students.begin(), students.end(), std::ostream_iterator<Person>(file, "\n"));

    // A full disk shows up here, not when the file is opened.
    if (!file)
        throw FileError("could not write to file \"" + fileName + "\"");
}

#endif
