// This file declares the exception classes the program throws when
// something goes wrong, so that main() can catch each kind separately.
#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

// This exception is thrown when a file cannot be opened or written.
class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& message) : std::runtime_error(message) {}
};

// This exception is thrown when a file opens but its content is not usable.
class DataError : public std::runtime_error {
public:
    explicit DataError(const std::string& message) : std::runtime_error(message) {}
};

#endif
