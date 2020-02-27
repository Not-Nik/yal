//
//  logger.hpp
//  CCord
//
//  Created by Nik Wipper on 09.05.19.
//  Copyright © 2019 Nikolas Wipper. All rights reserved.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <vector>
#include <iostream>

namespace log
{
    typedef enum { ERROR, WARNING, INFO, DEBUG } LOGGING_TYPE;

    class Logger
    {
        static std::vector<Logger *> loggers;

        std::string name;
        std::string layout = "[%l] %h:%m:%s: %i";
        LOGGING_TYPE currentType = INFO;
    public:
        /// Default constructor
        explicit Logger(std::string name);

        /// Dynamic constructor
        static Logger * getLogger(const std::string & name);
        static bool  deleteLogger(const std::string & name);

        /// Display message formatted with layout if current level is equals or larger than type
        void log(LOGGING_TYPE type, const std::string & message);

        // Functions wrapping the log() function
        void error(const std::string & message)   { this->log(ERROR,   message); }
        void warning(const std::string & message) { this->log(WARNING, message); }
        void info(const std::string & message)    { this->log(INFO,    message); }
        void debug(const std::string & message)   { this->log(DEBUG,   message); }

        /**
         * Set layout to a string. You can use certain codes to show different information.
         * All codes have to be marked with a '%':
         * l (Level): shows the level of the message (see LOGGING_TYPE)
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

    Logger::Logger(std::string name)
    {
        this->name = std::move(name);
    }

    void Logger::log(LOGGING_TYPE type, const std::string & message)
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
                else if (*fmt == 'l')
                {
                    if (type == ERROR)        std::cout << "ERROR";
                    else if (type == WARNING) std::cout << "WARNING";
                    else if (type == INFO)    std::cout << "INFO";
                    else if (type == DEBUG)   std::cout << "DEBUG";
                }
                else if (*fmt == '%') std::cout << "%";
            } else {
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
