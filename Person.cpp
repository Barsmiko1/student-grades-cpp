// This file contains the code of the Person methods declared in Person.h.
#include "Person.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <random>

// This constructor creates an empty student with all results set to zero.
Person::Person() : exam_(0), final_(0.0) {}

// This constructor creates a student with a name and surname, no results yet.
Person::Person(const std::string& name, const std::string& surname)
    : name_(name), surname_(surname), exam_(0), final_(0.0) {}

// This copy constructor creates a new student by copying every field of another one.
Person::Person(const Person& other)
    : name_(other.name_), surname_(other.surname_),
      homework_(other.homework_), exam_(other.exam_), final_(other.final_) {}

// This assignment operator copies every field of another student into this one.
Person& Person::operator=(const Person& other) {
    // Nothing to do when a student is assigned to itself (a = a).
    if (this == &other)
        return *this;

    name_ = other.name_;
    surname_ = other.surname_;
    homework_ = other.homework_;
    exam_ = other.exam_;
    final_ = other.final_;
    return *this;
}

// This destructor runs when a student is destroyed and releases its homework results.
Person::~Person() {
    homework_.clear();
}

// This method gives the student random homework results and a random exam result.
void Person::generateRandomScores(int homeworkCount) {
    // The generator is static, so it is created once and shared by all students.
    static std::mt19937 generator(std::random_device{}());
    // This makes every number from 1 to 10 equally likely.
    std::uniform_int_distribution<int> score(1, 10);

    homework_.clear();
    for (int i = 0; i < homeworkCount; ++i)
        homework_.push_back(score(generator));
    exam_ = score(generator);
}

// This method returns the average of the homework results (0 if there are none).
double Person::average() const {
    if (homework_.empty())
        return 0.0;
    return std::accumulate(homework_.begin(), homework_.end(), 0.0) / homework_.size();
}

// This method returns the median: the middle value of the sorted homework
// results, or the mean of the two middle values when their number is even.
double Person::median() const {
    if (homework_.empty())
        return 0.0;

    // A copy is sorted, so the original order of the results is kept.
    std::vector<int> sorted(homework_);
    std::sort(sorted.begin(), sorted.end());
    const std::size_t middle = sorted.size() / 2;
    if (sorted.size() % 2 == 0)
        return (sorted[middle - 1] + sorted[middle]) / 2.0;
    return sorted[middle];
}

// This method returns the final grade: 0.4 * homework + 0.6 * exam.
double Person::finalGrade(Method method) const {
    const double homework = (method == Method::Median) ? median() : average();
    return 0.4 * homework + 0.6 * exam_;
}

// This method calculates the final grade and stores it in the student.
void Person::calculateFinal(Method method) {
    final_ = finalGrade(method);
}

// This operator reads one student ("Name Surname HW1 ... HWn Exam") from a
// stream. If the data is wrong it marks the stream as failed and leaves the
// student unchanged.
std::istream& operator>>(std::istream& in, Person& p) {
    std::string name, surname;
    if (!(in >> name >> surname))
        return in;

    // This loop reads numbers until there is nothing left to read.
    std::vector<int> scores;
    int value;
    while (in >> value) {
        if (value < 0 || value > 10)
            return in.setstate(std::ios::failbit), in;
        scores.push_back(value);
    }

    // The loop above must have stopped because the input ran out, not
    // because of something that is not a number, and it needs at least the
    // exam result.
    if (!in.eof() || scores.empty())
        return in.setstate(std::ios::failbit), in;
    in.clear(std::ios::eofbit);

    // The last number is the exam; the rest are homework results.
    p.name_ = name;
    p.surname_ = surname;
    p.exam_ = scores.back();
    scores.pop_back();
    p.homework_ = scores;
    return in;
}

// This operator prints one table row: name and surname on the left, the
// final grade on the right with two decimal places.
std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(15) << p.name_
        << std::setw(15) << p.surname_
        << std::right << std::setw(18) << std::fixed << std::setprecision(2)
        << p.final_;
    return out;
}
