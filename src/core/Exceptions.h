#pragma once

#include <stdexcept>
#include <string>

class AppError : public std::runtime_error {
public:
    explicit AppError(const std::string& msg) : std::runtime_error(msg) {}
};

class CliError : public AppError {
public:
    explicit CliError(const std::string& msg) : AppError(msg) {}
};

class IoError : public AppError {
public:
    explicit IoError(const std::string& msg) : AppError(msg) {}
};

class WavFormatError : public AppError {
public:
    explicit WavFormatError(const std::string& msg) : AppError(msg) {}
};

class ConfigError : public AppError {
public:
    explicit ConfigError(const std::string& msg) : AppError(msg) {}
};
