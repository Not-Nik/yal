// yal (c) Nikolas Wipper 2020

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <vector>
#include <iostream>

#ifndef __GNUC__
#warning "This library requires built-in functions from GCC to properly display filename and line without you setting it explicitly"
#endif

namespace log
{
    typedef enum
    {
        ERROR, WARNING, INFO, DEBUG
    } LOGGING_TYPE;

    class Logger
    {
        static std::vector<Logger *> loggers;

        std::string name;
        std::string layout = "[%l] %h:%m:%s - %f:%c - %i";
        LOGGING_TYPE currentType = INFO;
    public:
        /// Default constructor
        explicit Logger(std::string n);

        /// Dynamic constructor
        static Logger * getLogger(const std::string & name);
        static bool deleteLogger(const std::string & name);

        /// Display message formatted with layout if current level is equals or larger than type
        void log(LOGGING_TYPE type, const std::string & message, const std::string & file = __builtin_FILE(), int line = __builtin_LINE());

        // Functions wrapping the log() function
        void error(const std::string & message, const std::string & file = __builtin_FILE(), int line = __builtin_LINE())
        { this->log(ERROR, message, file, line); }

        void warning(const std::string & message, const std::string & file = __builtin_FILE(), int line = __builtin_LINE())
        { this->log(WARNING, message, file, line); }

        void info(const std::string & message, const std::string & file = __builtin_FILE(), int line = __builtin_LINE())
        { this->log(INFO, message, file, line); }

        void debug(const std::string & message, const std::string & file = __builtin_FILE(), int line = __builtin_LINE())
        { this->log(DEBUG, message, file, line); }

        /**
         * Set layout to a string. You can use certain codes to show different information.
         * All codes have to be marked with a '%':
         * l (Level): shows the level of the message (see LOGGING_TYPE)
         * f (File): shows the filename
         * c (Column): shows the line
         * Time:
         * - h (Hour): shows the current hour
         * - m (Minute): shows the current minute
         * - s (Second): shows the current second
         * i (Information): shows the actual message
         */
        void setLayout(std::string newLayout);

        /// Set the logging type to type. It is used to determine which messages are shown and which don't.
        void setLoggingType(LOGGING_TYPE type);

        /// Get the name of this logger
        std::string getName();
    };

    Logger::Logger(std::string n) : name(n)
    {
    }

    void Logger::log(LOGGING_TYPE type, const std::string & message, const std::string & file, int line)
    {
        if (type > this->currentType)
            return;
        std::string outp;
        const char * fmt = this->layout.c_str();

        // Get the time
        time_t now = time(nullptr);
        tm * time = localtime(&now);

        while (fmt)
        {
            if (*fmt == '%')
            {
                fmt++;
                if (*fmt == 'h')      std::cout << time->tm_hour;
                else if (*fmt == 'm') std::cout << time->tm_min;
                else if (*fmt == 's') std::cout << time->tm_sec;
                else if (*fmt == 'i') std::cout << message;
                else if (*fmt == 'f') std::cout << file;
                else if (*fmt == 'c') std::cout << line;
                else if (*fmt == 'l')
                {
                    if (type == ERROR)        std::cout << "ERROR";
                    else if (type == WARNING) std::cout << "WARNING";
                    else if (type == INFO)    std::cout << "INFO";
                    else if (type == DEBUG)   std::cout << "DEBUG";
                }
                else if (*fmt == '%') std::cout << "%";
            }
            else
            {
                if (*fmt != '\0') std::cout << *fmt;
                else              break;
            }
            fmt++;
        }
        std::cout << std::endl;
    }

    void Logger::setLayout(std::string newLayout)
    {
        this->layout = std::move(newLayout);
    }

    void Logger::setLoggingType(LOGGING_TYPE type)
    {
        this->currentType = type;
    }

    std::string Logger::getName()
    {
        return this->name;
    }

    Logger * Logger::getLogger(const std::string & name)
    {
        for (auto * logger : Logger::loggers)
        {
            if (logger->getName() == name)
                return logger;
        }
        auto * l = new Logger(name);
        loggers.push_back(l);
        return l;
    }

    bool Logger::deleteLogger(const std::string & name)
    {
        std::vector<Logger *>::iterator i;
        for (i = loggers.begin(); i < loggers.end(); i++)
        {
            if ((*i)->getName() == name)
            {
                delete *i;
                loggers.erase(i);
                return true;
            }
        }
        return false;
    }

    std::vector<Logger *> Logger::loggers = std::vector<Logger *>();
}

#endif /*LOGGER_H*/
