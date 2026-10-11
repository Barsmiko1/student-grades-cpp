#include <iomanip>
#include <iostream>
#include <string>

#include "Person.h"

int main() {
    int count = 0;
    std::cout << "Number of students: ";
    if (!(std::cin >> count) || count <= 0) {
        std::cerr << "The number of students must be a positive integer.\n";
        return 1;
    }

    Person* students = new Person[count];
    for (int i = 0; i < count; ++i) {
        std::cout << "\nStudent " << i + 1 << "\n";
        if (!(std::cin >> students[i])) {
            std::cerr << "Invalid input.\n";
            delete[] students;
            return 1;
        }
        students[i].calculateFinal();
    }

    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << "Final_Point(Aver.)" << "\n"
              << std::string(48, '-') << "\n";
    for (int i = 0; i < count; ++i)
        std::cout << students[i] << "\n";

    delete[] students;
    return 0;
}
