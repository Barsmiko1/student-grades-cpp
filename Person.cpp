#include "Person.h"

#include <algorithm>
#include <iomanip>

Person::Person()
    : homework_(nullptr), homeworkCount_(0), exam_(0), final_(0.0) {}

Person::Person(const std::string& name, const std::string& surname)
    : name_(name), surname_(surname),
      homework_(nullptr), homeworkCount_(0), exam_(0), final_(0.0) {}

Person::Person(const Person& other)
    : name_(other.name_), surname_(other.surname_),
      homework_(nullptr), homeworkCount_(other.homeworkCount_),
      exam_(other.exam_), final_(other.final_) {
    if (homeworkCount_ > 0) {
        homework_ = new int[homeworkCount_];
        std::copy(other.homework_, other.homework_ + homeworkCount_, homework_);
    }
}

Person& Person::operator=(const Person& other) {
    if (this == &other)
        return *this;

    int* copy = nullptr;
    if (other.homeworkCount_ > 0) {
        copy = new int[other.homeworkCount_];
        std::copy(other.homework_, other.homework_ + other.homeworkCount_, copy);
    }
    delete[] homework_;

    name_ = other.name_;
    surname_ = other.surname_;
    homework_ = copy;
    homeworkCount_ = other.homeworkCount_;
    exam_ = other.exam_;
    final_ = other.final_;
    return *this;
}

Person::~Person() {
    delete[] homework_;
}

void Person::calculateFinal() {
    double average = 0.0;
    if (homeworkCount_ > 0) {
        double sum = 0.0;
        for (int i = 0; i < homeworkCount_; ++i)
            sum += homework_[i];
        average = sum / homeworkCount_;
    }
    final_ = 0.4 * average + 0.6 * exam_;
}

std::istream& operator>>(std::istream& in, Person& p) {
    // Prompts are only useful when a person is typing the data.
    const bool prompt = (&in == &std::cin);

    if (prompt) std::cout << "First name: ";
    in >> p.name_;
    if (prompt) std::cout << "Surname: ";
    in >> p.surname_;

    int count = 0;
    if (prompt) std::cout << "Number of homework assignments: ";
    in >> count;
    if (!in || count < 0) {
        in.setstate(std::ios::failbit);
        return in;
    }

    delete[] p.homework_;
    p.homework_ = count > 0 ? new int[count] : nullptr;
    p.homeworkCount_ = count;
    for (int i = 0; i < count; ++i) {
        if (prompt) std::cout << "Homework " << i + 1 << " result: ";
        in >> p.homework_[i];
    }

    if (prompt) std::cout << "Exam result: ";
    in >> p.exam_;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(15) << p.name_
        << std::setw(15) << p.surname_
        << std::right << std::setw(18) << std::fixed << std::setprecision(2)
        << p.final_;
    return out;
}
