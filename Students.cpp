#include "Students.h"

std::string strategyTitle(Strategy strategy) {
    switch (strategy) {
    case Strategy::Copy:
        return "Strategy 1 (copy into two new containers, for loop)";
    case Strategy::Erase:
        return "Strategy 2 (move failed out and erase, for loop)";
    case Strategy::CopyAlgorithms:
        return "Strategy 1 with algorithms (std::remove_copy_if + std::copy_if)";
    case Strategy::Partition:
        return "Strategy 2 with algorithms (std::partition)";
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
