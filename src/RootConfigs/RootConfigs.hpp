#ifndef __ROOT_CONFIGS__HPP__
#define __ROOT_CONFIGS__HPP__

#include <iostream>
#include <set>

struct RootConfigs {

    static const std::set<std::string> AvailableMethods;
    static const std::set<std::string> SupportedHttpProtocols;
    static const std::string InvalidHeaderKeys;

};

#endif // !__ROOT_CONFIGS__HPP__