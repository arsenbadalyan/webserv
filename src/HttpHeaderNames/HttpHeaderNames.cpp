#include "HttpHeaderNames.hpp"

HttpHeaderNames::HttpHeaderNames(void) {}
HttpHeaderNames::HttpHeaderNames(const HttpHeaderNames &) {}
HttpHeaderNames::~HttpHeaderNames() {}
void HttpHeaderNames::operator=(const HttpHeaderNames &) {}

const std::string HttpHeaderNames::CONTENT_LENGTH = "Content-Length";
const std::string HttpHeaderNames::CONTENT_TYPE = "Content-Type";
const std::string HttpHeaderNames::DATE = "Date";
const std::string HttpHeaderNames::SERVER = "Server";
const std::string HttpHeaderNames::EXPIRES = "Expires";
const std::string HttpHeaderNames::LAST_MODIFIED = "Last-Modified";
const std::string HttpHeaderNames::LOCATION = "Location";
const std::string HttpHeaderNames::CONNECTION = "Connection";
const std::string HttpHeaderNames::COOKIE = "Cookie";
const std::string HttpHeaderNames::TRANSFER_ENCODING = "Transfer-Encoding";
const std::string HttpHeaderNames::CONTENT_DISPOSITION = "Content-Disposition";