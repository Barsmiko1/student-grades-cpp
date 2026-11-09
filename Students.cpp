#include "Students.h"

bool byName(const Person& a, const Person& b) {
    if (a.name() != b.name())
        return a.name() < b.name();
    return a.surname() < b.surname();
}

bool byFinal(const Person& a, const Person& b) {
    if (a.finalResult() != b.finalResult())
        return a.finalResult() > b.finalResult();
    return byName(a, b);
}

void sortByName(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), byName);
}
