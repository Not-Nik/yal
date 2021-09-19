// yal (c) Nikolas Wipper 2020

#include <Logger.h>

using namespace log;

int main() {
    Logger *logger = Logger::GetLogger("MyLogger");
    logger->SetLoggingType(DEBUG);
    logger->Debug("Debug");
    logger->Info("Info");
    logger->Warning("Warning");
    logger->Error("Error");
    Logger::DeleteLogger("MyLogger");
}
