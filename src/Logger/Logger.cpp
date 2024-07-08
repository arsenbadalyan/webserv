#include "Logger.hpp"

const char* getCurrentTime() {
    std::time_t now = std::time(0);
    static char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %X", std::localtime(&now));
    return buf;
}

const char* getLogLevel(LogLevel level) {
    switch (level) {
        case LOGGER_INFO: return "\033[32mINFO\033[0m";  // Green
        case LOGGER_DEBUG: return "\033[34mDEBUG\033[0m"; // Blue
        case LOGGER_WARN: return "\033[33mWARN\033[0m";  // Yellow
        case LOGGER_ERROR: return "\033[31mERROR\033[0m"; // Red
        default: return "UNKNOWN";
    }
}

void logRequest(LogLevel level, std::string message) {
    std::cout << "[" << getCurrentTime() << "] [ " << getLogLevel(level) << " ] " << message << std::endl;
}