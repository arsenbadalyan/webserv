#ifndef __UTIL__HPP__
#define __UTIL__HPP__

#include <iostream>
#include <sstream>

typedef std::pair<std::string*, size_t> SplitPair;

class Util {

    public:
        static SplitPair split(const std::string& str, char delimiter);
        static std::string toLower(const std::string& str);

};

#endif // !__UTIL__HPP__