#ifndef __ROOT_CONFIGS__HPP__
#define __ROOT_CONFIGS__HPP__

#include <iostream>
#include <set>

struct RootConfigs {

    static const std::set<std::string> AvailableMethods;
    static const std::string SupportedHttpProtocol;
    static const std::string InvalidHeaderKeys;
    static const std::string Whitespaces;
    static const std::string SERVER_NAME;
    static const std::string TERMINATION_BUFFER;
    static const std::string TERMINATION_CHARS;

};

#endif // !__ROOT_CONFIGS__HPP__