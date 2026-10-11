// This file contains the sorting and splitting functions declared in Students.h.
#include "Students.h"

#include <algorithm>

// This function tells std::sort which of two students comes first by name.
static bool byName(const Person& a, const Person& b) {
    if (a.name() != b.name())
        return a.name() < b.name();
    return a.surname() < b.surname();
}

// This function tells std::sort which of two students comes first by grade:
// the higher final grade first, and by name when the grades are equal.
static bool byFinal(const Person& a, const Person& b) {
    if (a.finalResult() != b.finalResult())
        return a.finalResult() > b.finalResult();
    return byName(a, b);
}

void sortByName(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), byName);
}

void sortByFinal(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), byFinal);
}

void splitStudents(const std::vector<Person>& students,
                   std::vector<Person>& passed, std::vector<Person>& failed) {
    for (const Person& student : students) {
        if (student.finalResult() < PASS_MARK)
            failed.push_back(student);
        else
            passed.push_back(student);
    }
}
