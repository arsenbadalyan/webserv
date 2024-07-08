#include "HttpStatusCode.hpp"

HttpStatusCode::HttpStatusCode(void) {}
HttpStatusCode::HttpStatusCode(const HttpStatusCode &) {}
HttpStatusCode::~HttpStatusCode() {}
void HttpStatusCode::operator=(const HttpStatusCode &) {}

// Define the enumeration values
const int16_t HttpStatusCode::OK;
const int16_t HttpStatusCode::CREATED;
const int16_t HttpStatusCode::BAD_GATEWAY;
const int16_t HttpStatusCode::NOT_ALLOWED;
const int16_t HttpStatusCode::CONTENT_TOO_LARGE;
const int16_t HttpStatusCode::MOVED_PERMANENTLY;
const int16_t HttpStatusCode::INTERNAL_SERVER_ERROR;
const int16_t HttpStatusCode::FORBIDDEN;
const int16_t HttpStatusCode::NOT_FOUND;

const std::pair<int, std::string> statusCodesKeyValuePairs[] = {
    std::make_pair(HttpStatusCode::OK, "OK"),
    std::make_pair(HttpStatusCode::CREATED, "Created"),
    std::make_pair(HttpStatusCode::MOVED_PERMANENTLY, "Moved Permanently"),
    std::make_pair(HttpStatusCode::FORBIDDEN, "Forbidden"),
    std::make_pair(HttpStatusCode::NOT_FOUND, "Not Found"),
    std::make_pair(HttpStatusCode::NOT_ALLOWED, "Not Allowed"),
    std::make_pair(HttpStatusCode::CONTENT_TOO_LARGE, "Content Too Large"),
    std::make_pair(HttpStatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"),
    std::make_pair(HttpStatusCode::BAD_GATEWAY, "Bad Gateway")
};

const StatusCodesMap HttpStatusCode::StatusCode(
    statusCodesKeyValuePairs,
    statusCodesKeyValuePairs + (sizeof(statusCodesKeyValuePairs) / sizeof(statusCodesKeyValuePairs[0]))
);

std::string HttpStatusCode::getStatusCode(const int16_t& code) {
    return (HttpStatusCode::StatusCode.find(code)->second);
}

bool HttpStatusCode::isErrorStatusCode(const int16_t& code) {
    return (code >= 400 && code <= 599);
}

bool HttpStatusCode::isRedirectStatusCode(const int16_t& code) {
    return (code >= 300 && code < 400);
}