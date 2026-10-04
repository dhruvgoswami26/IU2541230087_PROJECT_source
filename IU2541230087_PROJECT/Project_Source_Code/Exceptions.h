// Exceptions.h - custom exception classes used across the Security Gate Pass System
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base class for all application-specific errors
class GateException : public std::runtime_error {
public:
    explicit GateException(const std::string& msg) : std::runtime_error(msg) {}
};

class ValidationException : public GateException {
public:
    explicit ValidationException(const std::string& msg) : GateException("Validation failed: " + msg) {}
};

class DuplicateException : public GateException {
public:
    explicit DuplicateException(const std::string& msg) : GateException("Duplicate record: " + msg) {}
};

class NotFoundException : public GateException {
public:
    explicit NotFoundException(const std::string& msg) : GateException("Not found: " + msg) {}
};

class IntegrityException : public GateException {
public:
    explicit IntegrityException(const std::string& msg) : GateException("Operation not allowed: " + msg) {}
};

class PassException : public GateException {
public:
    explicit PassException(const std::string& msg) : GateException("Pass rejected: " + msg) {}
};

class FileException : public GateException {
public:
    explicit FileException(const std::string& msg) : GateException("File error: " + msg) {}
};

// Thrown when standard input is closed (Ctrl+Z / Ctrl+D / end of piped input)
class InputEnded : public std::runtime_error {
public:
    InputEnded() : std::runtime_error("Input stream closed") {}
};

#endif
