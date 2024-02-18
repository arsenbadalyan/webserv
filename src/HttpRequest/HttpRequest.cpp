#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest::~HttpRequest() {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

HttpRequest::HttpRequest(int fd) {
	std::string readRes = this->readSocket(fd);
}

std::string HttpRequest::readSocket(int fd) const {
	ssize_t readRes;
	std::string resultStr = "";
	char buffer[BUFFER_SIZE];

	while (true) {
		bzero(&buffer, strlen(buffer));
		readRes = recv(fd, buffer, BUFFER_SIZE, 0);

		if (readRes <= 0)
			break ;

		resultStr += std::string(buffer);

		if (strnstr(buffer, TERMINATION_BUFFER, strlen(buffer)))
			break ;
	}

	return (resultStr);
}

void HttpRequest::requestStartLineParser(std::string & request) {
	std::istringstream iss(request);
	std::string firstLine;

	std::getline(iss, firstLine);

	


	request.erase(0, firstLine.length() + 1);
}