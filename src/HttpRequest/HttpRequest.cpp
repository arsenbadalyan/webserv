#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest::~HttpRequest() {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

HttpRequest::HttpRequest(Server* currentServer, int readSocketFd) :
	boundary(""),
	contentLength(0),
	chunking(chunk_type::no_chunks),
	_hasFinishedRead(false),
	_server(currentServer)
{
	std::cout << "<<<<<<<<<<<< NEW REQUEST" << std::endl;
	this->requestInitialParsing(readSocketFd);

	if (this->chunking == chunk_type::no_chunks)
		this->_hasFinishedRead = true;

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
	char buffer[READ_BUFFER_SIZE];

	while (true) {
		bzero(&buffer, (READ_BUFFER_SIZE * sizeof(char)));
		readBytes = recv(socket, buffer, READ_BUFFER_SIZE, 0);

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

HttpRequest& HttpRequest::requestInitialParsing(int fd) {
	ssize_t readRes;
	std::string resultStr;
	char buffer[READ_BUFFER_SIZE];
	bool isAlreadyReadStartLine = false;
	char *terminationBufferPtr = NULL;
	size_t terminationBufferPos = 0;

	while (true) {
		bzero(&buffer, (READ_BUFFER_SIZE * sizeof(char)));
		readRes = recv(fd, buffer, READ_BUFFER_SIZE, 0);

		if (readRes <= 0)
			break ;

		resultStr += std::string(buffer);
		if (!isAlreadyReadStartLine) {
			this->requestStartLineParser(resultStr);
			isAlreadyReadStartLine = true;
		}

		terminationBufferPtr = strnstr(resultStr.c_str(), TERMINATION_BUFFER, strlen(resultStr.c_str()));
		if (terminationBufferPtr) {
			terminationBufferPos = terminationBufferPtr - resultStr.c_str();
			std::string headersSection = resultStr.substr(0, terminationBufferPos);

			this->parseHeadersBuffer(headersSection);
			this->configureRequestByHeaders();
			if (this->contentLength) {
				terminationBufferPos += strlen(TERMINATION_BUFFER);
				resultStr = resultStr.substr(terminationBufferPos, strlen(resultStr.c_str() - terminationBufferPos));
			}
			break ;
		}
	}

	if (this->contentLength) {
		if (this->chunking == chunk_type::no_chunks)
			this->extractBody(fd, resultStr);
		else
			this->extractChunk(fd, resultStr);
	}

	return (*this);
}

HttpRequest& HttpRequest::extractBody(int sockFd, std::string initialData) {
	if (initialData.length() > this->contentLength)
		this->body = initialData.substr(0, this->contentLength);
	else if (initialData.length() == this->contentLength)
		this->body = initialData;
	else {
		size_t remainingBytes = this->contentLength - initialData.length();
		char *buffer = new char[remainingBytes];
		recv(sockFd, buffer, remainingBytes, 0);
		this->body = initialData + std::string(buffer);
		delete[] buffer;
	}

	return (*this);
}

HttpRequest& HttpRequest::extractChunk(int sockFd, std::string initialData) {
	if (!this->hasEndBoundary(initialData)) {
		char buffer[READ_BUFFER_SIZE];

		while (true) {
			recv(sockFd, buffer, READ_BUFFER_SIZE, 0);
			initialData += std::string(buffer);

			if (this->hasEndBoundary(initialData))
				break ;
		}
	}
	return (*this);
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

	if (splittedFirstLineSize >= 3
		&& Util::toLower(splittedFirstLine[2]) != RootConfigs::SupportedHttpProtocol) {
			throw std::runtime_error(std::string(ERR_NOT_SUPPORTED_HTTP_PROTOCOL) + Util::toLower(splittedFirstLine[2]));
		}
	
	this->method = splittedFirstLine[0];
	this->endpoint = splittedFirstLine[1];

	request.erase(0, lineSize + 1);
}

void HttpRequest::configureRequestByHeaders(void) {
	
	const std::string* contentType = this->headers.getHeader(HttpHeaderNames::CONTENT_TYPE);
	const std::string* transferEncoding = this->headers.getHeader(HttpHeaderNames::TRANSFER_ENCODING);
	const std::string* contentLength = this->headers.getHeader(HttpHeaderNames::CONTENT_LENGTH);

	if (contentType) {
		SplitPair splitRes = Util::split(*contentType, ';');

		if (splitRes.second >= 1) {
			this->contentType = splitRes.first[0];
		}

		if (splitRes.second >= 2
			&& splitRes.first[0].find("multipart/form-data") != std::string::npos
			&& splitRes.first[1].find("boundary=") != std::string::npos) {
				SplitPair boundary = Util::split(Util::trim(splitRes.first[1], RootConfigs::Whitespaces), '=');

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

	if (contentLength) {
		this->contentLength = Util::strToSizeT(*contentLength);
	}
}

bool HttpRequest::hasFinishedReceivingRequest(void) const {
	return (this->_hasFinishedRead);
}

const std::string HttpRequest::getEndpoint(void) const {
	return (this->endpoint);
}

const std::string HttpRequest::getFullFilePath(void) const {
	return (this->_server->getServerConfig().getRoot() + this->getEndpoint());
}

Server * HttpRequest::getServer(void) const {
	return (this->_server);
}

const std::string HttpRequest::getMethod(void) const {
	return (this->method);
}

// returns empty string if header not found
std::string HttpRequest::getHeader(std::string headerName) const {
	const std::string* headerValue = this->headers.getHeader(headerName);

	if (!headerValue)	
		return ("");

	return (*headerValue);
}


// private stuff
bool HttpRequest::hasStartBoundary(const std::string& str) {
	size_t pos = str.find(this->boundary);

	if (pos == std::string::npos || pos)
		return (false);
	return (true);
}

bool HttpRequest::hasEndBoundary(const std::string& str) {
	std::string searchTail = "--\r\n";
	size_t pos = str.find(this->boundary + searchTail);

	if (pos == std::string::npos)
		return (false);
	return (true);
}