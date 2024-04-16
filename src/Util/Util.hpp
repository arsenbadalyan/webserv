#ifndef __UTIL__HPP__
#define __UTIL__HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sys/stat.h>
#include <dirent.h>
#include "ExceptionHandler.hpp"
#include "HttpStatusCode.hpp"
#include "RootConfigs.hpp"

typedef std::pair<std::string*, size_t> SplitPair;

class Util {

	public:
		static SplitPair split(const std::string& str, char delimiter);
		static std::string toLower(const std::string& str);
		static std::string& trim(std::string& str, const std::string& trimmerStr); 
		static size_t getFileSize(std::istream& is);
		static bool			in(const std::string& s1, const std::string& s2);
		static std::string getDateTime(std::time_t &currentTime);
		static std::string getDateTime();
		static std::string checkFileLastModifyDate(const std::string& filepath);
		static bool isDirectory(const char *path);
		static std::string extractFileType(const std::string & path);
		static std::string generateDefaultErrorPage(const int16_t & statusCode);
		static std::string intToString(int value);

};

#endif // !__UTIL__HPP__