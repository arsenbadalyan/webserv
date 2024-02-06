#include "HttpHeaders.hpp"

HttpHeaders::HttpHeaders(void) {}
HttpHeaders::HttpHeaders(const HttpHeaders &) {}
HttpHeaders::~HttpHeaders() { this->headers.clear(); }
void HttpHeaders::operator=(const HttpHeaders &) {}

const std::string* HttpHeaders::getHeader(std::string & key) const {
	HeadersMap::const_iterator foundedKey = this->headers.find(key);

	if (foundedKey == this->headers.end())
		return (NULL);

	return (&foundedKey->second);
}

HttpHeaders& HttpHeaders::setHeader(std::string & key, std::string & value) {
	this->headerValidator(key, value);
	this->headers[key] = value;

	return (*this);
}

HttpHeaders& HttpHeaders::setHeader(std::string & line) {
	std::string key, value;
	size_t separatorPos;

	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	separatorPos = line.find(':');

	if (separatorPos == std::string::npos) {
		key = line;
		value = "";
	} else {
		key = line.substr(0, separatorPos);
		value = line.substr(separatorPos + 1, line.length());
	}

	return (this->setHeader(key, value));
}