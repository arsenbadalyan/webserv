#include "ExceptionHandler.hpp"

void ExceptionHandler::WrongHeader(void) {
	throw std::runtime_error("One of request headers are invalid.");
}

void ExceptionHandler::InvalidHeaderKey(void) {
	throw std::runtime_error("One of request headers contains invalid character.");
}

void ExceptionHandler::UnexpectedSendRequest(void) {
	throw std::runtime_error("Unexpected error occurred while sending request.");
}

void ExceptionHandler::CannotGetRequestToSendResponse(void) {
	throw std::runtime_error("There are no such request socket to send response to the client.");
}

void ExceptionHandler::CannotDestroyRequest(void) {
    throw std::runtime_error("There are no such request to destroy.");
}

void ExceptionHandler::InvalidFileInfo(void) {
    throw std::runtime_error("Unable to get file information.");
}
