#include "Students.h"

std::string strategyTitle(Strategy strategy) {
    switch (strategy) {
    case Strategy::Copy:
        return "Strategy 1 (copy into two new containers, for loop)";
    case Strategy::Erase:
        return "Strategy 2 (move failed out and erase, for loop)";
    }
    return "";
}

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
