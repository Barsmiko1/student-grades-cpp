// This file declares the functions that read students from a file, write
// them to a file and generate random test data files. Reading and writing
// are templates, so they work for std::vector, std::list and std::deque.
#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Errors.h"
#include "Person.h"

// This function returns the column title for the chosen method.
std::string finalTitle(Method method);

// This function creates a data file with studentCount students named
// Name1 Surname1, Name2 Surname2, ... and random results from 1 to 10.
// It throws FileError if the file cannot be created.
void generateStudentFile(const std::string& fileName, int studentCount, int homeworkCount);

// This function reads all students from a text file into a container of the
// requested type, for example readStudentsFromFile<std::list<Person>>(name).
// The first line is a header and every other line is
// "Name Surname HW1 ... HWn Exam". It throws FileError if the file cannot
// be opened and DataError if the file contains no students.
template <typename Container>
Container readStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file)
        throw FileError("cannot open file \"" + fileName + "\"");

    std::string line;
    std::getline(file, line); // skip the header line

    Container students;
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

// This function writes a table (name, surname, stored final grade) to a
// file. It throws FileError if the file cannot be created or written.
template <typename Container>
void writeStudentsToFile(const std::string& fileName, const Container& students, Method method) {
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

#endif
