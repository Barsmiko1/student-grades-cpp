// This file declares the Person class: what a student stores and what can
// be done with it. The code of the methods is in Person.cpp.
#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

// This enum names the two ways the homework part of the grade is calculated.
enum class Method { Average, Median };

// This class describes one student: name, surname, homework results,
// exam result and the calculated final grade.
class Person {
public:
    // This constructor creates an empty student.
    Person();
    // This constructor creates a student with a name and surname, no results yet.
    Person(const std::string& name, const std::string& surname);

    // Rule of three: if a class defines one of these, it should define all three.
    // This copy constructor creates a new student as a copy of another one.
    Person(const Person& other);
    // This assignment operator copies another student into an existing one (a = b).
    Person& operator=(const Person& other);
    // This destructor cleans up when a student is destroyed.
    ~Person();

    // This method fills in homeworkCount random homework results and a
    // random exam result, all in the range 1..10.
    void generateRandomScores(int homeworkCount);

    // This method returns the final grade without storing it:
    // Final = 0.4 * (average | median of homework) + 0.6 * exam
    double finalGrade(Method method) const;
    // This method calculates the final grade and stores it in the student.
    void calculateFinal(Method method);

    // These methods let other code read the name and surname (used for sorting).
    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }

    // This operator reads one student: "Name Surname HW1 HW2 ... HWn Exam".
    // The number of homework results is not fixed - every number up to the
    // end of the input is read and the last one is taken as the exam result.
    friend std::istream& operator>>(std::istream& in, Person& p);
    // This operator prints one table row: name, surname and final grade.
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

private:
    // These methods return the average and the median of the homework results.
    double average() const;
    double median() const;

    std::string name_;
    std::string surname_;
    std::vector<int> homework_; // homework results, any number of them
    int exam_;
    double final_;              // final grade, set by calculateFinal()
};

#endif
