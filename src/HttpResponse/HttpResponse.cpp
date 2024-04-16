#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const HttpResponse&) {}
void HttpResponse::operator=(const HttpResponse&) {}

HttpResponse::HttpResponse(const HttpRequest * request, int writeSocketFd)
	: _server(request->getServer()),
	_request(request),
	_writeSocketFd(writeSocketFd),
	_requestedFile(request->getFullFilePath()),
	_statusCode(HttpStatusCode::INVALID_STATUS_CODE)
{
	std::cout << "FULL PATH -> " << request->getFullFilePath() << std::endl;
	std::cout << "ENDPOINT -> " << request->getEndpoint() << std::endl;
	this->_requestedFilePath = request->getFullFilePath();
	this->_configs = this->_server->findLocations(request->getEndpoint());
	this->_isReturnTerminatedResponse = (this->_configs->getReturn().getStatusTypes() == NO_CUSTOM_STATUS_CODE);
}

HttpResponse::~HttpResponse() {
	this->_requestedFile.close();
}

void HttpResponse::getResponse(void) {
	std::string response = "";

	response += HttpResponse::configureStatusLine();
	this->_requestedFileType = Util::extractFileType(this->_requestedFilePath);
	HttpResponse::configureHeaders();
	if (HttpStatusCode::isErrorStatusCode(this->_statusCode)) {
		std::cout << "I AM HERE START" << std::endl;
		this->_requestedFile.close();
		std::map<int, std::string> errorPages = this->_configs->getError_page();
		std::cout << "I AM HERE" << std::endl;
		std::cout << "CHECK ERROR PAGE KEY -> " << errorPages.begin()->first << std::endl;
		std::cout << "CHECK ERROR PAGE VALUE -> " << errorPages.begin()->second << std::endl;
		std::map<int, std::string>::iterator errorPageIt = errorPages.find(this->_statusCode);
		std::map<int, std::string>::iterator it = errorPages.begin();

		while (it != errorPages.end()) {
			std::cout << it->first << " <--> " << it->second << std::endl;
			it++;
		}

		if (errorPageIt != errorPages.end()) {
			std::cout << "<<<<<<<<<< File NOT FOUND IS CUSTOM" << std::endl;
			std::cout << "FILE NAME IS: " << errorPageIt->second << std::endl;
			this->_requestedFile.open(errorPageIt->second);
			if (this->_requestedFile.is_open()) {
				std::cout << "<<<<<<<<<< File NOT FOUND OPENED SUCCESSFULLY" << std::endl;
				response += this->_headers.toString() + "\r\n\r\n";
				send(this->_writeSocketFd, response.c_str(), response.length() * sizeof(char), 0);
				HttpResponse::sendBody();
				return ;
			}
			std::cout << "<<<<<<<<<< File NOT FOUND OPENED WITH FAILURE" << std::endl;
		}
		std::string errorPageHtml = Util::generateDefaultErrorPage(this->_statusCode) + "\r\n\r\n";

		this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::intToString(errorPageHtml.length()));
		response += this->_headers.toString() + "\r\n\r\n";
		send(this->_writeSocketFd, response.c_str(), response.length() * sizeof(char), 0);
		send(this->_writeSocketFd, errorPageHtml.c_str(), errorPageHtml.length() * sizeof(char), 0);
	} else {
		response += this->_headers.toString() + "\r\n\r\n";
		send(this->_writeSocketFd, response.c_str(), response.length() * sizeof(char), 0);
		HttpResponse::sendBody();
	}


}

std::string HttpResponse::configureStatusLine(void) {
	int16_t statusCode = HttpStatusCode::INVALID_STATUS_CODE;
	std::string statusLineStr = "";
	std::stringstream ss;

	if (this->_requestedFile.is_open()) {

		if (Util::isDirectory(this->_request->getFullFilePath().c_str())) {
			std::vector<std::string> indexFiles = this->_configs->getIndex();
			std::vector<std::string>::iterator indexFilesIt = indexFiles.begin();

			this->_requestedFile.close();
			while (indexFilesIt != indexFiles.end()) {
				this->_requestedFile.open(*indexFilesIt);
				if (this->_requestedFile.is_open()) {
					statusCode = HttpStatusCode::OK;
					this->_requestedFilePath = *indexFilesIt;
					break ;
				}
			}

			if (statusCode == HttpStatusCode::INVALID_STATUS_CODE) {
				if (this->_configs->getAutoindex()) {
					// TODO: make directory listing
				} else {
					statusCode = HttpStatusCode::FORBIDDEN;
				}
			}
		} else {
			ss << HttpStatusCode::OK;
		}

	} else {
		statusCode = HttpStatusCode::NOT_FOUND;
	}

	statusLineStr += RootConfigs::SupportedHttpProtocol + " ";
	statusLineStr += Util::intToString(statusCode) + " ";
	statusLineStr += HttpStatusCode::getStatusCode(statusCode) + "\r\n";
	this->_statusCode = statusCode;

	return (statusLineStr);
}

void HttpResponse::configureDefaultHeaders(void) {
	std::string currentDate = Util::getDateTime();

	this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, std::string("0"));
	this->_headers.setHeader(HttpHeaderNames::SERVER, std::string(RootConfigs::SERVER_NAME));
	this->_headers.setHeader(HttpHeaderNames::DATE, currentDate);
	this->_headers.setHeader(HttpHeaderNames::EXPIRES, currentDate);
	if (this->_requestedFile.is_open()) {
		this->_headers.setHeader(HttpHeaderNames::LAST_MODIFIED, Util::checkFileLastModifyDate(this->_requestedFilePath));
	}
}

std::string HttpResponse::configureHeaders() {
	this->configureDefaultHeaders();

	if (this->_requestedFile.is_open()) {

		std::stringstream ss;
		std::string key, value;
		size_t fileLength = Util::getFileSize(this->_requestedFile);

		ss << fileLength;
		std::getline(ss, value);
		key = std::string("Content-Length");
		this->_headers.setHeader(key, value);
	}

	return (this->_headers.toString() + "\r\n");
}

void HttpResponse::sendBody() {
	std::string line;
	std::streamsize bytesRead;
	char buffer[1024];
	size_t length = 0;

	(void)length;
	(void)this->_request;
	std::cout << "<<<<<<<<<< SENDING BODY" << std::endl;
	bzero(&buffer, sizeof(buffer));
	while ((bytesRead = this->_requestedFile.read(buffer, sizeof(buffer)).gcount()) > 0) {
		std::cout << strlen(buffer) << std::endl;
		length += strlen(buffer);
		send(this->_writeSocketFd, &buffer, sizeof(buffer), 0);
		bzero(&buffer, sizeof(buffer));
	}
	std::cout << "<<<<<<<<<< FINISHED SENDING BODY" << std::endl;
}
