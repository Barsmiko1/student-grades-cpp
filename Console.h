// This file declares the functions that talk to the user: asking for input
// on the keyboard and printing tables on the screen.
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <vector>

#include "Person.h"

// This function keeps asking until the user types a whole number between
// min and max, and returns that number.
int readNumber(const std::string& question, int min, int max);

// This function asks the user to choose average or median.
Method chooseMethod();

// This function lets the user type students one per line, with as many
// homework results as they want. An empty line ends the input.
std::vector<Person> enterStudents();

// This function lets the user type only names and surnames; the homework
// and exam results of every student are generated randomly.
std::vector<Person> generateStudents();

// This function prints the table with the stored final grade of every student.
void printFinals(const std::vector<Person>& students, Method method);

// This function prints the table with both the average and the median
// based final grade of every student.
void printBothFinals(const std::vector<Person>& students);

#endif
