#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest::~HttpRequest() {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

HttpRequest::HttpRequest(int fd) {
	std::string readRes = this->parseRequest(fd);

	

	// HeadersMap::iterator it = this->headers
	// while ()
}

std::string HttpRequest::parseRequest(int fd) {
	ssize_t readRes;
	std::string resultStr;
	char buffer[BUFFER_SIZE];
	bool isAlreadyReadStartLine = false;

	while (true) {
		bzero(&buffer, strlen(buffer));
		readRes = recv(fd, buffer, BUFFER_SIZE, 0);

		if (readRes <= 0)
			break ;

		resultStr += std::string(buffer);
		if (!isAlreadyReadStartLine) {
			this->requestStartLineParser(resultStr);
			isAlreadyReadStartLine = true;
		}

		if (strnstr(buffer, TERMINATION_BUFFER, strlen(buffer))) {
			this->parseBuffer(resultStr);
			break ;
		}
	}

	return (resultStr);
}

void HttpRequest::parseBuffer(std::string & buffer) {
	std::stringstream is(buffer);
	std::string readlineOutput;

	while (std::getline(is, readlineOutput)) {
		headers.setHeader(readlineOutput);
	}
}

void HttpRequest::requestStartLineParser(std::string & request) {
	std::istringstream iss(request);
	std::string firstLine;

	std::getline(iss, firstLine);

	SplitPair splittedFirstLinePair = Util::split(firstLine, ' ');
	std::string *splittedFirstLine = splittedFirstLinePair.first;
	size_t splittedFirstLineSize = splittedFirstLinePair.second;

	if (splittedFirstLineSize < 2) {
		throw std::runtime_error(ERR_INVALID_REQUEST);
	}

	if (RootConfigs::AvailableMethods.find(Util::toLower(splittedFirstLine[0])) == RootConfigs::AvailableMethods.end()) {
		throw std::runtime_error(ERR_NOT_SUPPORTED_HTTP_METHOD);
	}

	// TODO: url parser
	// if (splittedFirstLine.second[1])

	// if (splittedFirstLineSize >= 3
	// 	&& RootConfigs::SupportedHttpProtocols.find(Util::toLower(splittedFirstLine[2])) == RootConfigs::SupportedHttpProtocols.end()) {
	// 		std::cout << (RootConfigs::SupportedHttpProtocols.find(Util::toLower(splittedFirstLine[2])) == RootConfigs::SupportedHttpProtocols.end()) << std::endl;
	// 		throw std::runtime_error(std::string(ERR_NOT_SUPPORTED_HTTP_PROTOCOL) + Util::toLower(splittedFirstLine[2]));
	// 	}

	request.erase(0, firstLine.length() + 1);
}
