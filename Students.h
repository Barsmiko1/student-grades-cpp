// This file declares the functions that work on a whole list of students:
// sorting it and dividing it into passed and failed.
#ifndef STUDENTS_H
#define STUDENTS_H

#include <vector>

#include "Person.h"

// A student with a final grade below this mark has failed.
const double PASS_MARK = 5.0;

// This function sorts students by name, then by surname.
void sortByName(std::vector<Person>& students);

// This function sorts students by their stored final grade, best first.
// Students with the same grade are ordered by name.
void sortByFinal(std::vector<Person>& students);

// This function divides students into two new containers: "passed"
// (final grade >= 5.0) and "failed" (final grade < 5.0). It uses the final
// grade stored by calculateFinal() and keeps the order of the students.
void splitStudents(const std::vector<Person>& students,
                   std::vector<Person>& passed, std::vector<Person>& failed);

#endif
