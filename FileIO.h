// This file declares the functions that read students from a file, write
// them to a file and generate random test data files.
#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>

#include "Person.h"

// This function reads all students from a text file. The first line is a
// header and every other line is "Name Surname HW1 ... HWn Exam".
// It throws FileError if the file cannot be opened and DataError if the
// file contains no students.
std::vector<Person> readStudentsFromFile(const std::string& fileName);

// This function writes a table (name, surname, stored final grade) to a
// file. It throws FileError if the file cannot be created.
void writeStudentsToFile(const std::string& fileName,
                         const std::vector<Person>& students, Method method);

// This function creates a data file with studentCount students named
// Name1 Surname1, Name2 Surname2, ... and random results from 1 to 10.
// It throws FileError if the file cannot be created.
void generateStudentFile(const std::string& fileName, int studentCount, int homeworkCount);

// This function returns the column title for the chosen method.
std::string finalTitle(Method method);

#endif
