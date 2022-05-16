// yal (c) Nikolas Wipper 2020

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <utility>
#include <vector>
#include <iostream>
#include <filesystem>

#ifndef __GNUC__
#ifdef _MSC_VER
#pragma message ("This library requires built-in functions from GCC to properly display filename and line without you setting it explicitly")
#else
#warning "This library requires built-in functions from GCC to properly display filename and line without you setting it explicitly"
#endif
#endif

namespace logging {
typedef enum {
    ERROR, WARNING, INFO, DEBUG
} LOGGING_TYPE;

// This is very naive, but seems to work fine
[[maybe_unused]] constexpr std::string_view MakeRelative(std::string_view path) {
    std::string_view current = __builtin_FILE(), res;
    std::string_view::value_type separator = 0;
    for (auto el : path) {
        if (el == '/' || el == '\\') {
            separator = el;
            break;
        }
    }

    size_t i = 0, size = std::min(current.size(), path.size());
    for (; i < size; i++) {
        if (current[i] != path[i]) break;
    }
    return path.substr(i);
}

class Logger {
    static inline std::vector<Logger *> loggers;

    std::string name;
    std::string layout = "[%l] %n: %h:%m:%s - %p:%c (%f) - %i";
    LOGGING_TYPE currentType = INFO;
public:
    /// Default constructor
    inline explicit Logger(std::string n);

    /// Dynamic constructor
    static inline Logger *GetLogger(const std::string &name);

    static inline bool DeleteLogger(const std::string &name);

    /// Display message formatted with layout if current level is equals or larger than type
    inline void Log(LOGGING_TYPE type,
                    const std::string &message,
                    const std::string_view &file = MakeRelative(__builtin_FILE()),
                    int line = __builtin_LINE(),
                    const std::string &function = __builtin_FUNCTION());

    // Functions wrapping the log() function
    void Error(const std::string &message,
               const std::string_view &file = MakeRelative(__builtin_FILE()),
               int line = __builtin_LINE(),
               const std::string &function = __builtin_FUNCTION()) { this->Log(ERROR, message, file, line, function); }

    void Warning(const std::string &message,
                 const std::string_view &file = MakeRelative(__builtin_FILE()),
                 int line = __builtin_LINE(),
                 const std::string &function = __builtin_FUNCTION()) { this->Log(WARNING, message, file, line, function); }

    void Info(const std::string &message,
              const std::string_view &file = MakeRelative(__builtin_FILE()),
              int line = __builtin_LINE(),
              const std::string &function = __builtin_FUNCTION()) { this->Log(INFO, message, file, line, function); }

    void Debug(const std::string &message,
               const std::string_view &file = MakeRelative(__builtin_FILE()),
               int line = __builtin_LINE(),
               const std::string &function = __builtin_FUNCTION()) { this->Log(DEBUG, message, file, line, function); }

    /**
     * Set layout to a string. You can use certain codes to show different information.
     * All codes have to be marked with a '%':
     * l (Level): shows the level of the message (see LOGGING_TYPE)
     * n (Name): shows the loggers name
     * p (Path): shows the file path
     * c (Column): shows the line
     * f (Function): shows the function name
     * Time:
     * - h (Hour): shows the current hour
     * - m (Minute): shows the current minute
     * - s (Second): shows the current second
     * i (Information): shows the actual message
     */
    inline void SetLayout(std::string new_layout);

    /// Set the logging type to type. It is used to determine which messages are shown and which don't.
    inline void SetLoggingType(LOGGING_TYPE type);

    /// Get the name of this logger
    inline std::string GetName();
};

Logger::Logger(std::string n)
    : name(std::move(n)) {
}

void Logger::Log(LOGGING_TYPE type, const std::string &message, const std::string_view &file, int line, const std::string &function) {
    if (type > this->currentType)
        return;
    std::string outp;
    const char *fmt = this->layout.c_str();

    // Get the time
    time_t now = time(nullptr);
    tm *time = localtime(&now);

    while (fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'h') std::cout << std::setw(2) << std::setfill('0') << time->tm_hour;
            else if (*fmt == 'm') std::cout << std::setw(2) << std::setfill('0') << time->tm_min;
            else if (*fmt == 's') std::cout << std::setw(2) << std::setfill('0') << time->tm_sec;
            else if (*fmt == 'i') std::cout << message;
            else if (*fmt == 'p') std::cout << file;
            else if (*fmt == 'c') std::cout << line;
            else if (*fmt == 'f') std::cout << function;
            else if (*fmt == 'n') std::cout << name;
            else if (*fmt == 'l') {
                if (type == ERROR) std::cout << "ERROR";
                else if (type == WARNING) std::cout << "WARNING";
                else if (type == INFO) std::cout << "INFO";
                else if (type == DEBUG) std::cout << "DEBUG";
            } else if (*fmt == '%') std::cout << "%";
        } else {
            if (*fmt != '\0') std::cout << *fmt;
            else break;
        }
        fmt++;
    }
    std::cout << std::endl;
}

void Logger::SetLayout(std::string new_layout) {
    this->layout = std::move(new_layout);
}

void Logger::SetLoggingType(LOGGING_TYPE type) {
    this->currentType = type;
}

std::string Logger::GetName() {
    return this->name;
}

Logger *Logger::GetLogger(const std::string &name) {
    for (auto *logger: Logger::loggers) {
        if (logger->GetName() == name)
            return logger;
    }
    auto *l = new Logger(name);
    loggers.push_back(l);
    return l;
}

bool Logger::DeleteLogger(const std::string &name) {
    std::vector<Logger *>::iterator i;
    for (i = loggers.begin(); i < loggers.end(); i++) {
        if ((*i)->GetName() == name) {
            delete *i;
            loggers.erase(i);
            return true;
        }
    }
    return false;
}
} // logging

#endif /*LOGGER_H*/
