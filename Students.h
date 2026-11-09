// Sorting students and dividing them into passed and failed. Templates, so
// the same code works for std::vector, std::list and std::deque.
#ifndef STUDENTS_H
#define STUDENTS_H

#include <algorithm>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include "Person.h"

const double PASS_MARK = 5.0;

enum class Strategy {
    Copy,           // Strategy 1: copy into new "passed" and "failed", base keeps everyone
    Erase,          // Strategy 2: move failed into "failed", erase them from the base
    CopyAlgorithms, // Strategy 1 with std::remove_copy_if / std::copy_if
    Partition       // Strategy 2 with std::partition
};

// Short names for the command line, in the same order as the enum.
const std::vector<std::string> STRATEGIES = {"copy", "erase", "copy-alg", "partition"};

std::string strategyTitle(Strategy strategy);

inline bool isFailed(const Person& student) { return student.finalResult() < PASS_MARK; }
inline bool isPassed(const Person& student) { return !isFailed(student); }

bool byName(const Person& a, const Person& b);
// Higher final grade first, by name when equal.
bool byFinal(const Person& a, const Person& b);

void sortByName(std::vector<Person>& students);

template <typename Container>
void sortByFinal(Container& students) {
    std::sort(students.begin(), students.end(), byFinal);
}

// std::sort needs random access, so a list uses its own sort().
inline void sortByFinal(std::list<Person>& students) {
    students.sort(byFinal);
}

// Gives back the memory of removed students (vector and deque keep it otherwise).
template <typename Container>
void shrinkToFit(Container& students) {
    students.shrink_to_fit();
}

inline void shrinkToFit(std::list<Person>&) {}

// Strategy 1: every student is copied, the base container is not changed.
template <typename Container>
void splitCopy(const Container& students, Container& passed, Container& failed) {
    for (const Person& student : students) {
        if (isFailed(student))
            failed.push_back(student);
        else
            passed.push_back(student);
    }
}

// Strategy 2: afterwards the base container holds only the passed students.
// erase() in the middle of a vector or deque shifts every student behind
// it, so this is quadratic for those two containers.
template <typename Container>
void splitErase(Container& students, Container& failed) {
    auto it = students.begin();
    while (it != students.end()) {
        if (isFailed(*it)) {
            failed.push_back(std::move(*it));
            it = students.erase(it);
        } else {
            ++it;
        }
    }
    shrinkToFit(students);
}

// Strategy 1 with algorithms: remove_copy_if copies everyone who is NOT
// failed, copy_if copies everyone who is.
template <typename Container>
void splitCopyAlgorithms(const Container& students, Container& passed, Container& failed) {
    std::remove_copy_if(students.begin(), students.end(), std::back_inserter(passed), isFailed);
    std::copy_if(students.begin(), students.end(), std::back_inserter(failed), isFailed);
}

// Strategy 2 with algorithms: partition moves the passed students to the
// front in one pass, then the failed tail is moved out and erased at once.
// The order inside the groups changes, but both are sorted afterwards, so
// the cheaper partition is used instead of stable_partition.
template <typename Container>
void splitPartition(Container& students, Container& failed) {
    const auto firstFailed = std::partition(students.begin(), students.end(), isPassed);
    failed.assign(std::make_move_iterator(firstFailed), std::make_move_iterator(students.end()));
    students.erase(firstFailed, students.end());
    shrinkToFit(students);
}

// A list can hand its failed nodes over with splice() without moving any student.
inline void splitPartition(std::list<Person>& students, std::list<Person>& failed) {
    const auto firstFailed = std::partition(students.begin(), students.end(), isPassed);
    failed.splice(failed.end(), students, firstFailed, students.end());
}

#endif
