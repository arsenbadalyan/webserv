#include "RootConfigs.hpp"

const std::string AvailableMethodsList[] = { "get", "post", "delete" };

const std::string RootConfigs::InvalidHeaderKeys = "()<>@,;:\\\"/[]?={} ";

const std::string RootConfigs::Whitespaces = " \n\t\v\f\r";

const std::set<std::string> RootConfigs::AvailableMethods(
	AvailableMethodsList,
	AvailableMethodsList + (sizeof(AvailableMethodsList) / sizeof(AvailableMethodsList[0]))
);

const std::string RootConfigs::SupportedHttpProtocol = "http/1.1";

const std::string RootConfigs::SERVER_NAME = "webserv";

const std::string RootConfigs::TERMINATION_CHARS = "\r\n";

const std::string RootConfigs::TERMINATION_BUFFER = RootConfigs::TERMINATION_CHARS + RootConfigs::TERMINATION_CHARS;
