#ifndef __WEBSERV_LOGGER__HPP__
#define __WEBSERV_LOGGER__HPP__

#include <iostream>
#include <string>
#include <ctime>

enum LogLevel { LOGGER_INFO, LOGGER_DEBUG, LOGGER_WARN, LOGGER_ERROR };

const char* getCurrentTime();
const char* getLogLevel(LogLevel level);
void logRequest(LogLevel level, std::string message);

#endif // !__WEBSERV_LOGGER__HPP__