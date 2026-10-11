// This file declares the functions that work on a whole list of students:
// sorting it and dividing it into passed and failed. The functions are
// templates, so the same code works for std::vector, std::list and std::deque.
#ifndef STUDENTS_H
#define STUDENTS_H

#include <algorithm>
#include <list>
#include <vector>

#include "Person.h"

// A student with a final grade below this mark has failed.
const double PASS_MARK = 5.0;

// This function tells a sort which of two students comes first by name.
bool byName(const Person& a, const Person& b);

// This function tells a sort which of two students comes first by grade:
// the higher final grade first, and by name when the grades are equal.
bool byFinal(const Person& a, const Person& b);

// This function sorts students by name, then by surname.
void sortByName(std::vector<Person>& students);

// This function sorts students by their stored final grade, best first.
// It works for std::vector and std::deque.
template <typename Container>
void sortByFinal(Container& students) {
    std::sort(students.begin(), students.end(), byFinal);
}

// This version is chosen for std::list: std::sort cannot jump around in a
// list, so the list has its own sort() method.
inline void sortByFinal(std::list<Person>& students) {
    students.sort(byFinal);
}

// This function divides students into two new containers: "passed"
// (final grade >= 5.0) and "failed" (final grade < 5.0). It uses the final
// grade stored by calculateFinal() and keeps the order of the students.
template <typename Container>
void splitStudents(const Container& students, Container& passed, Container& failed) {
    for (const Person& student : students) {
        if (student.finalResult() < PASS_MARK)
            failed.push_back(student);
        else
            passed.push_back(student);
    }
}

#endif
