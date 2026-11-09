// Exceptions thrown by the program and caught in main().
#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

// A file cannot be opened, created or written.
class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& message) : std::runtime_error(message) {}
};

// A file opens but contains no usable students.
class DataError : public std::runtime_error {
public:
    explicit DataError(const std::string& message) : std::runtime_error(message) {}
};

#endif
