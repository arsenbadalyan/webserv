#ifndef __UTIL__HPP__
#define __UTIL__HPP__

#include <iostream>
#include <sstream>
#include <string>

typedef std::pair<std::string*, size_t> SplitPair;

class Util {

    public:
        static SplitPair split(const std::string& str, char delimiter);
        static std::string toLower(const std::string& str);
        static std::string& trim(std::string& str, const std::string& trimmerStr); 
        static size_t getFileSize(std::istream& is);
		static bool			in(const std::string& s1, const std::string& s2);

};

#endif // !__UTIL__HPP__