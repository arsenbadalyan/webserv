#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest::~HttpRequest() {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

HttpRequest::HttpRequest(int fd) :
	boundary(""),
	chunking(chunk_type::no_chunks)
{
	std::string readRes = this->requestInitialParsing(fd);

	this->configureRequestByHeaders();

	std::cout << "<<<<<<<<<<<< REQUEST RESULTS" << std::endl;
	std::cout << "method: " << this->method << std::endl;
	std::cout << "endpoint: " << this->endpoint << std::endl;
	std::cout << "isChunked: " << this->chunking << std::endl;
	std::cout << "boundary: " << this->boundary << std::endl;
	std::cout << "content type: " << this->contentType << std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
}

void HttpRequest::parseMultipartDataForm(int socket) {

	ssize_t readBytes;
	std::string readRes = "";
	bool hasAlreadyFoundBoundary = false;
	char buffer[BUFFER_SIZE];

	while (true) {
		bzero(&buffer, (BUFFER_SIZE * sizeof(char)));
		readBytes = recv(socket, buffer, BUFFER_SIZE, 0);

		if (readBytes <= 0)
			break ;

		readRes += std::string(buffer);
		if (readRes.find(this->boundary) != std::string::npos) {
			if (hasAlreadyFoundBoundary) {
				std::istringstream is(readRes);
				std::string cutPart;
				std::getline(is, cutPart);
				std::cout << cutPart << std::endl;
			}
		}

	}

}

std::string HttpRequest::requestInitialParsing(int fd) {
	ssize_t readRes;
	std::string resultStr;
	char buffer[BUFFER_SIZE];
	bool isAlreadyReadStartLine = false;

	while (true) {
		bzero(&buffer, (BUFFER_SIZE * sizeof(char)));
		readRes = recv(fd, buffer, BUFFER_SIZE, 0);

		if (readRes <= 0)
			break ;

		resultStr += std::string(buffer);
		if (!isAlreadyReadStartLine) {
			this->requestStartLineParser(resultStr);
			isAlreadyReadStartLine = true;
		}

		if (strnstr(buffer, TERMINATION_BUFFER, strlen(buffer))) {
			this->parseHeadersBuffer(resultStr);
			break ;
		}
	}

	return (resultStr);
}

void HttpRequest::parseHeadersBuffer(std::string & buffer) {
	std::stringstream is(buffer);
	std::string readlineOutput;

	while (std::getline(is, readlineOutput)) {
		headers.setHeader(readlineOutput);
	}
}

void HttpRequest::requestStartLineParser(std::string & request) {
	std::istringstream iss(request);
	std::string firstLine;
	size_t lineSize;

	std::getline(iss, firstLine);

	lineSize = firstLine.length();
	Util::trim(firstLine, RootConfigs::Whitespaces);
	std::cout << firstLine << std::endl;
	SplitPair splittedFirstLinePair = Util::split(firstLine, ' ');
	std::string *splittedFirstLine = splittedFirstLinePair.first;
	size_t splittedFirstLineSize = splittedFirstLinePair.second;

	if (splittedFirstLineSize < 2) {
		throw std::runtime_error(ERR_INVALID_REQUEST);
	}

	if (RootConfigs::AvailableMethods.find(Util::toLower(splittedFirstLine[0])) == RootConfigs::AvailableMethods.end()) {
		throw std::runtime_error(ERR_NOT_SUPPORTED_HTTP_METHOD);
	}

	// TODO: url parser if needed
	// if (splittedFirstLine.second[1])

	if (splittedFirstLineSize >= 3
		&& RootConfigs::SupportedHttpProtocols.find(Util::toLower(splittedFirstLine[2])) == RootConfigs::SupportedHttpProtocols.end()) {
			throw std::runtime_error(std::string(ERR_NOT_SUPPORTED_HTTP_PROTOCOL) + Util::toLower(splittedFirstLine[2]));
		}
	
	this->method = splittedFirstLine[0];
	this->endpoint = splittedFirstLine[1];

	request.erase(0, lineSize + 1);
}

void HttpRequest::configureRequestByHeaders(void) {
	
	const std::string* contentType = this->headers.getHeader("content-type");
	const std::string* transferEncoding = this->headers.getHeader("transfer-encoding");

	if (contentType) {
		SplitPair splitRes = Util::split(*contentType, ';');

		if (splitRes.second >= 1) {
			this->contentType = splitRes.first[0];
		}

		if (splitRes.second >= 2
			&& splitRes.first[0].find("multipart/form-data") != std::string::npos
			&& splitRes.first[1].find("boundary=") != std::string::npos) {
				SplitPair boundary = Util::split(Util::trim(splitRes.first[2], RootConfigs::Whitespaces), '=');

				if (boundary.second == 2) {
					this->chunking = chunk_type::dataForm_chunk;
					this->boundary = boundary.first[1];
				}
		}
	}

	if (this->chunking == chunk_type::no_chunks
		&& transferEncoding
		&& *transferEncoding == "chunked") {
		this->chunking = chunk_type::encoding_chunk; 
	}
}