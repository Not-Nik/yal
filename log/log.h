// yal (c) Nikolas Wipper 2020

#ifndef LOGGER_H
#define LOGGER_H

#include <map>
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
    NONE, ERROR, WARNING, INFO, DEBUG, ALL
} LOGGING_TYPE;

// This is very naive, but seems to work fine
[[maybe_unused]] constexpr std::string_view MakeRelative(std::string_view path) {
    std::string_view current = __builtin_FILE();

    size_t i = 0, size = std::min(current.size(), path.size());
    for (; i < size; i++) {
        if (current[i] != path[i]) break;
    }
    return path.substr(i);
}

class Logger {
    static inline std::string name;
    static inline std::string layout = "[%l] %n: %h:%m:%s - %p:%c (%f) - %i";
    static inline LOGGING_TYPE currentType = INFO;
public:

    /// Display message formatted with layout if current level is equals or larger than type
    static inline void Log(LOGGING_TYPE type,
                        const std::string &message,
                        const std::string_view &file = MakeRelative(__builtin_FILE()),
                        int line = __builtin_LINE(),
                        const std::string &function = __builtin_FUNCTION());

    // Functions wrapping the log() function
    static inline void Error(const std::string &message,
                          const std::string_view &file = MakeRelative(__builtin_FILE()),
                          int line = __builtin_LINE(),
                          const std::string &function = __builtin_FUNCTION()) {
        Log(ERROR, message, file, line, function);
    }

    static inline void Warning(const std::string &message,
                            const std::string_view &file = MakeRelative(__builtin_FILE()),
                            int line = __builtin_LINE(),
                            const std::string &function = __builtin_FUNCTION()) {
        Log(WARNING, message, file, line, function);
    }

    static inline void Info(const std::string &message,
                         const std::string_view &file = MakeRelative(__builtin_FILE()),
                         int line = __builtin_LINE(),
                         const std::string &function = __builtin_FUNCTION()) {
        Log(INFO, message, file, line, function);
    }

    static inline void Debug(const std::string &message,
                          const std::string_view &file = MakeRelative(__builtin_FILE()),
                          int line = __builtin_LINE(),
                          const std::string &function = __builtin_FUNCTION()) {
        Log(DEBUG, message, file, line, function);
    }

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
     * - t (Relative time): shows the time in seconds since the process started
     * i (Information): shows the actual message
     */
    static inline void SetLayout(std::string new_layout);

    /// Set the logging type to type. It is used to determine which messages are shown and which aren't.
    static inline void SetLoggingType(LOGGING_TYPE type);

    /// Get the name of this logger
    static inline std::string GetName();

    /// Set the name of this logger
    static inline void SetName(std::string new_name);
};

void Logger::Log(LOGGING_TYPE type,
                 const std::string &message,
                 const std::string_view &file,
                 int line,
                 const std::string &function) {
    if (type > currentType)
        return;
    std::string outp;
    const char *fmt = layout.c_str();

    // Get the time
    time_t now = time(nullptr);
    tm *time = localtime(&now);

    while (fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'h') std::cout << std::setw(2) << std::setfill('0') << time->tm_hour;
            else if (*fmt == 'm') std::cout << std::setw(2) << std::setfill('0') << time->tm_min;
            else if (*fmt == 's') std::cout << std::setw(2) << std::setfill('0') << time->tm_sec;
            else if (*fmt == 't') std::cout << static_cast<float>(clock()) / static_cast<float>(CLOCKS_PER_SEC);
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
    layout = std::move(new_layout);
}

void Logger::SetLoggingType(LOGGING_TYPE type) {
    currentType = type;
}

std::string Logger::GetName() {
    return name;
}

void Logger::SetName(std::string new_name) {
    name = std::move(new_name);
}
} // logging

#endif /*LOGGER_H*/
