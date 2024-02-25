#include "RootConfigs.hpp"

const std::string AvailableMethodsList[] = { "get", "post", "delete" };
const std::string SupportedHttpProtocolsList[] = { "http/1.1" };

const std::string RootConfigs::InvalidHeaderKeys = "()<>@,;:\\\"/[]?={} ";

const std::string RootConfigs::Whitespaces = " \n\t\v\f\r";

const std::set<std::string> RootConfigs::AvailableMethods(
    AvailableMethodsList,
    AvailableMethodsList + (sizeof(AvailableMethodsList) / sizeof(AvailableMethodsList[0]))
);

const std::set<std::string> RootConfigs::SupportedHttpProtocols(
    SupportedHttpProtocolsList,
    SupportedHttpProtocolsList + (sizeof(SupportedHttpProtocolsList) / sizeof(SupportedHttpProtocolsList[0]))
);