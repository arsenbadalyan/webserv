#include "ExceptionHandler.hpp"

void ExceptionHandler::WrongHeader(void) {
    throw std::runtime_error("One of request headers are invalid.");
}

void ExceptionHandler::InvalidHeaderKey(void) {
    throw std::runtime_error("One of request headers contains invalid character.");
}