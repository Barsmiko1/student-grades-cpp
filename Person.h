// Person: one student with homework results, an exam result and a final grade.
#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

enum class Method { Average, Median };

class Person {
public:
    Person();
    Person(const std::string& name, const std::string& surname);

    // Rule of three
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Moving instead of copying makes sorting millions of students cheap.
    Person(Person&& other) noexcept;
    Person& operator=(Person&& other) noexcept;

    // Random homework and exam results in the range 1..10.
    void generateRandomScores(int homeworkCount);

    // Final = 0.4 * (average | median of homework) + 0.6 * exam
    double finalGrade(Method method) const;
    void calculateFinal(Method method);

    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }
    double finalResult() const { return final_; }

    // Reads "Name Surname HW1 ... HWn Exam"; the last number is the exam.
    friend std::istream& operator>>(std::istream& in, Person& p);
    // Prints one table row: name, surname, final grade.
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

private:
    double average() const;
    double median() const;

    std::string name_;
    std::string surname_;
    std::vector<int> homework_;
    int exam_;
    double final_;
};

#endif
