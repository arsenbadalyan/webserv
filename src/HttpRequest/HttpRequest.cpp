#include "HttpRequest.hpp"
#include <iostream>

HttpRequest::HttpRequest(void) {}
HttpRequest::HttpRequest(const HttpRequest &) {}
HttpRequest::~HttpRequest() {}
HttpRequest& HttpRequest::operator=(const HttpRequest &) { return (*this); }

HttpRequest::HttpRequest(Server* currentServer, int readSocketFd) :
	body(""),
	boundary(""),
	contentLength(0),
	chunking(chunk_type::no_chunks),
	_hasFinishedRead(false),
	_server(currentServer),
	receivedBytes(0),
	_hasReadChunkedDataHeaders(false),
	_isUploadedFile(false),
	_fd(readSocketFd)
{
	::logRequest(LOGGER_INFO, "New Connection received from socket " + Util::intToString(readSocketFd));
	fcntl(readSocketFd, F_SETFL, O_NONBLOCK);
	this->requestInitialParsing(readSocketFd);
	this->makeChunkRegularCheck();

	if (this->chunking == chunk_type::no_chunks)
		this->_hasFinishedRead = true;

	// std::cout << "method: " << this->method << std::endl;
	// std::cout << "endpoint: " << this->endpoint << std::endl;
	// std::cout << "isChunked: " << this->chunking << std::endl;
	// std::cout << "boundary: " << this->boundary << std::endl;
	// std::cout << "content type: " << this->contentType << std::endl;
	// std::cout << "<<<<<<< IS FINISHED READ ?? " << this->_hasFinishedRead << std::endl;
}

void HttpRequest::prepareRead(int socketFd) {

	::logRequest(LOGGER_INFO, "New Chunk received from socket " + Util::intToString(socketFd));
	ssize_t readBytes = 0;
	std::string readRes = "";
	char buffer[READ_BUFFER_SIZE];

	while (true) {
		bzero(&buffer, (READ_BUFFER_SIZE * sizeof(char)));
		readBytes = recv(socketFd, buffer, READ_BUFFER_SIZE, 0);
		if (readBytes < 0) {
			break ;
		}

		if (readBytes == 0) {
			break ; 
		}

		receivedBytes += static_cast<double>(readBytes) / 1048576.0;

		this->body.append(buffer, readBytes);
	}

	this->makeChunkRegularCheck();
	// std::cout << "<<<<<<< IS FINISHED READ ?? " << this->_hasFinishedRead << std::endl;
}

HttpRequest& HttpRequest::makeChunkRegularCheck(void) {

	if (this->chunking == chunk_type::dataForm_chunk) {
		if (!this->_hasReadChunkedDataHeaders) {

			size_t terminationBufferPos = this->body.find(RootConfigs::TERMINATION_BUFFER);

			if (terminationBufferPos != std::string::npos) {
				std::string fileHeaders = this->body.substr(0, terminationBufferPos + RootConfigs::TERMINATION_BUFFER.length());
				size_t cursorPos = 0, cursorPrevPos = 0;
				bool alreadyFoundBoundary = false;
				std::string headersLineStr = "", tempHeader = "";

				this->body.erase(0, terminationBufferPos + RootConfigs::TERMINATION_BUFFER.length());
				do {

					cursorPrevPos = cursorPos;
					cursorPos = fileHeaders.find(RootConfigs::TERMINATION_CHARS, cursorPos);

					if (cursorPos != std::string::npos && cursorPrevPos != cursorPos) {
						cursorPos += RootConfigs::TERMINATION_CHARS.length();
						if (alreadyFoundBoundary) {
							
							tempHeader = fileHeaders.substr(cursorPrevPos, cursorPos - cursorPrevPos - RootConfigs::TERMINATION_CHARS.length());
							this->_chunkedDataHeaders.setHeader(tempHeader);

						} else alreadyFoundBoundary = true;
					}

				} while (cursorPos <= fileHeaders.length() && cursorPos != cursorPrevPos);

				this->_hasReadChunkedDataHeaders = true;
			}
		}

		size_t endBoundaryPos = this->hasEndBoundary(this->body);
		if (endBoundaryPos != std::string::npos) {
			this->body.erase(endBoundaryPos, std::string::npos);
			this->_hasFinishedRead = true;
			this->_isUploadedFile = true;
		}
		::logRequest(LOGGER_INFO, "From client " + Util::intToString(this->_fd) + " received " + Util::intToString(receivedBytes) + "mb");
	}

	return (*this);
}

HttpRequest& HttpRequest::requestInitialParsing(int fd) {
	ssize_t readRes = 0;
	std::string resultStr;
	char buffer[READ_BUFFER_SIZE];
	bool isAlreadyReadStartLine = false, isAlreadyFoundHeaders = false;
	size_t terminationBufferPos = 0;


	while (true) {
		bzero(&buffer, (READ_BUFFER_SIZE * sizeof(char)));
		readRes = recv(fd, buffer, READ_BUFFER_SIZE, 0);

		if (readRes < 0) {
			break ;
		}

		if (readRes == 0)
			break ;

		receivedBytes += static_cast<double>(readRes) / 1048576.0;

		this->body.append(buffer, readRes);

		if (!isAlreadyReadStartLine && this->body.find("\r\n") != std::string::npos) {
			this->requestStartLineParser(this->body);
			isAlreadyReadStartLine = true;
		}

		if (!isAlreadyReadStartLine)
			continue ;

		if (isAlreadyFoundHeaders)
			continue ;

		terminationBufferPos = this->body.find(RootConfigs::TERMINATION_BUFFER);

		if (terminationBufferPos != std::string::npos) {
			std::string headersSection = this->body.substr(0, terminationBufferPos);
			this->parseHeadersBuffer(headersSection);
			this->configureRequestByHeaders();
			this->body.erase(0, terminationBufferPos + RootConfigs::TERMINATION_BUFFER.length());
			isAlreadyFoundHeaders = true;
		}
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
		this->headers.setHeader(readlineOutput);
	}
}

void HttpRequest::requestStartLineParser(std::string & request) {
	std::istringstream iss(request);
	std::string firstLine;
	size_t lineSize;

	std::getline(iss, firstLine);

	lineSize = firstLine.length();
	Util::trim(firstLine, RootConfigs::Whitespaces);
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

	::logRequest(LOGGER_DEBUG, "Client " + Util::intToString(this->_fd) + " requested [" + this->method + "] " + this->endpoint);

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

size_t HttpRequest::hasEndBoundary(const std::string& str) {
	size_t pos = str.find("\r\n--" + this->boundary + "--\r\n");

	return (pos);
}

bool HttpRequest::isFileUpload(void) const {
	return (this->_isUploadedFile);
}

const std::string& HttpRequest::getBody(void) const {
	return (this->body);
}

const HttpHeaders& HttpRequest::getChunkedDataHeaders(void) const {
	return  (this->_chunkedDataHeaders);
}