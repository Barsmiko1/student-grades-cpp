// Keyboard input and tables on the screen.
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <vector>

#include "Person.h"

// Asks again until the user types a whole number between min and max.
int readNumber(const std::string& question, int min, int max);

Method chooseMethod();

// One student per line, any number of homework results; an empty line ends input.
std::vector<Person> enterStudents();

// Only names are typed; the results are generated randomly.
std::vector<Person> generateStudents();

void printFinals(const std::vector<Person>& students, Method method);
void printBothFinals(const std::vector<Person>& students);

#endif
