#include "FileIO.h"

#include <random>

std::string finalTitle(Method method) {
    return (method == Method::Average) ? "Final_Point(Aver.)" : "Final_Point(Med.)";
}

void generateStudentFile(const std::string& fileName, int studentCount, int homeworkCount) {
    std::ofstream file(fileName);
    if (!file)
        throw FileError("cannot create file \"" + fileName + "\"");

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> score(1, 10);

    file << std::left << std::setw(20) << "Name" << std::setw(20) << "Surname" << std::right;
    for (int i = 1; i <= homeworkCount; ++i)
        file << std::setw(6) << ("HW" + std::to_string(i));
    file << std::setw(6) << "Exam" << "\n";

    for (int i = 1; i <= studentCount; ++i) {
        file << std::left << std::setw(20) << ("Name" + std::to_string(i))
             << std::setw(20) << ("Surname" + std::to_string(i)) << std::right;
        for (int j = 0; j <= homeworkCount; ++j) // the extra number is the exam
            file << std::setw(6) << score(generator);
        file << "\n";
    }

    if (!file)
        throw FileError("could not write to file \"" + fileName + "\"");
}
