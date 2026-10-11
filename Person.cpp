#include "Person.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <random>

Person::Person() : exam_(0), final_(0.0) {}

Person::Person(const std::string& name, const std::string& surname)
    : name_(name), surname_(surname), exam_(0), final_(0.0) {}

Person::Person(const Person& other)
    : name_(other.name_), surname_(other.surname_),
      homework_(other.homework_), exam_(other.exam_), final_(other.final_) {}

Person& Person::operator=(const Person& other) {
    if (this == &other)
        return *this;

    name_ = other.name_;
    surname_ = other.surname_;
    homework_ = other.homework_;
    exam_ = other.exam_;
    final_ = other.final_;
    return *this;
}

Person::~Person() {
    homework_.clear();
}

void Person::generateRandomScores(int homeworkCount) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> score(1, 10);

    homework_.clear();
    for (int i = 0; i < homeworkCount; ++i)
        homework_.push_back(score(generator));
    exam_ = score(generator);
}

double Person::average() const {
    if (homework_.empty())
        return 0.0;
    return std::accumulate(homework_.begin(), homework_.end(), 0.0) / homework_.size();
}

double Person::median() const {
    if (homework_.empty())
        return 0.0;

    std::vector<int> sorted(homework_);
    std::sort(sorted.begin(), sorted.end());
    const std::size_t middle = sorted.size() / 2;
    if (sorted.size() % 2 == 0)
        return (sorted[middle - 1] + sorted[middle]) / 2.0;
    return sorted[middle];
}

double Person::finalGrade(Method method) const {
    const double homework = (method == Method::Median) ? median() : average();
    return 0.4 * homework + 0.6 * exam_;
}

void Person::calculateFinal(Method method) {
    final_ = finalGrade(method);
}

std::istream& operator>>(std::istream& in, Person& p) {
    std::string name, surname;
    if (!(in >> name >> surname))
        return in;

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

    p.name_ = name;
    p.surname_ = surname;
    p.exam_ = scores.back();
    scores.pop_back();
    p.homework_ = scores;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left << std::setw(15) << p.name_
        << std::setw(15) << p.surname_
        << std::right << std::setw(18) << std::fixed << std::setprecision(2)
        << p.final_;
    return out;
}
