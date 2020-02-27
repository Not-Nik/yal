// yal (c) Nikolas Wipper 2020

#include <Logger.h>

using namespace log;

int main()
{
    Logger * logger = Logger::getLogger("myLogger");
    logger->setLoggingType(DEBUG);
    logger->debug("Debug");
    logger->info("Info");
    logger->warning("Warning");
    logger->error("Error");
    Logger::deleteLogger("myLogger");
}
