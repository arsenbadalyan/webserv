#ifndef __UTIL__HPP__
#define __UTIL__HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sys/stat.h>
#include "ExceptionHandler.hpp"

typedef std::pair<std::string*, size_t> SplitPair;

class Util {

    public:
        static SplitPair split(const std::string& str, char delimiter);
        static std::string toLower(const std::string& str);
        static std::string& trim(std::string& str, const std::string& trimmerStr); 
        static size_t getFileSize(std::istream& is);
        static std::string getDateTime(std::time_t &currentTime);
        static std::string getDateTime();
        static std::string checkFileLastModifyDate(const std::string& filepath);
};

#endif // !__UTIL__HPP__