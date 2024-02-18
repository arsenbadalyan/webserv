#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

std::string HttpRequest::readSocket(int fd) {
	ssize_t readRes;
	std::string resultStr = "";
	char buffer[BUFFER_SIZE];

	while (true) {
		readRes = recv(fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);

		if (readRes <= 0 || strnstr(buffer, TERMINATION_BUFFER, strlen(TERMINATION_BUFFER)))
			break ;
		
		resultStr += buffer;
	}

	return (resultStr);
}