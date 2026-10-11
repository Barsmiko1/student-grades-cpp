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

    // Fills in homeworkCount random homework results and a random exam
    // result, all in the range 1..10.
    void generateRandomScores(int homeworkCount);

    // Final = 0.4 * (average | median of homework) + 0.6 * exam
    double finalGrade(Method method) const;
    void calculateFinal(Method method);

    // Reads one student: "Name Surname HW1 HW2 ... HWn Exam".
    // The number of homework results is not fixed - every number up to the
    // end of the input is read and the last one is taken as the exam result.
    friend std::istream& operator>>(std::istream& in, Person& p);
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
