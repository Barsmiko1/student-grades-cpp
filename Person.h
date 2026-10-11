#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

class Person {
public:
    Person();
    Person(const std::string& name, const std::string& surname);

    // Rule of three: the class owns a dynamic array of homework results.
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Final = 0.4 * average of homework + 0.6 * exam
    void calculateFinal();

    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);

private:
    std::string name_;
    std::string surname_;
    int* homework_;
    int homeworkCount_;
    int exam_;
    double final_;
};

#endif
