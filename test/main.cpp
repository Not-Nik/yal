// yal (c) Nikolas Wipper 2020

#include <log/log.h>

using namespace logging;

int main() {
    Logger::SetLoggingType(DEBUG);
    Logger::Debug("Debug");
    Logger::Info("Info");
    Logger::Warning("Warning");
    Logger::Error("Error");
}
