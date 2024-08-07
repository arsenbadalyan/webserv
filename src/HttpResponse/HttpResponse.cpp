#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const HttpResponse&) {}
void HttpResponse::operator=(const HttpResponse&) {}

HttpResponse::HttpResponse(const HttpRequest * request, int writeSocketFd)
	: _server(request->getServer()),
	_request(request),
	_writeSocketFd(writeSocketFd),
	_requestedFile(request->getFullFilePath()),
	_statusCode(HttpStatusCode::INVALID_STATUS_CODE),
	_folderStructure(NULL)
{
	::logRequest(LOGGER_INFO,
		"Sending response -> Client: "
		+ Util::intToString(writeSocketFd) + ", "
		+ "Full path: "
		+ request->getFullFilePath() + ", "
		+ "Endpoint: "
		+ request->getEndpoint());

	this->_requestedFilePath = request->getFullFilePath();
	this->_configs = this->_server->findLocations(request->getEndpoint());
	this->_isReturnTerminatedResponse = !(this->_configs->getReturn().getStatusTypes() == NO_CUSTOM_STATUS_CODE);
}

HttpResponse::~HttpResponse() {
	this->_requestedFile.close();
	if (this->_folderStructure) {
		delete this->_folderStructure;
	}
}

void HttpResponse::getResponse(void) {

	try {

		if (this->_request->isFileUpload()) {
			try {
				HttpResponse::configureDefaultHeaders();
				this->_statusCode = HttpStatusCode::CREATED;
				const std::string* contentDisposition = this->_request->getChunkedDataHeaders().getHeader(HttpHeaderNames::CONTENT_DISPOSITION);

				if (contentDisposition) {
					std::map<std::string, std::string> UploadFileData = HttpHeaders::getContentDispositionData(*contentDisposition);

					std::string fileName = (*UploadFileData.find("filename")).second;
					std::ofstream uploadedFile(fileName);

					if (uploadedFile.is_open()) {
						uploadedFile << this->_request->getBody();
					} else {
						this->_statusCode = HttpStatusCode::INTERNAL_SERVER_ERROR;
					}

				} else {
					this->_statusCode = HttpStatusCode::INTERNAL_SERVER_ERROR;
				}
			} catch(std::exception &exc) {
				this->_statusCode = HttpStatusCode::INTERNAL_SERVER_ERROR;
			}
			this->sendResponseRootSlice();
			return ;
		}

		if (this->_isReturnTerminatedResponse) {
			this->_statusCode = this->_configs->getReturn().getStatusTypes();
			HttpResponse::configureDefaultHeaders();
			if (HttpStatusCode::isRedirectStatusCode(this->_statusCode)
				&& this->_configs->getReturn().getPath())
				this->_headers.setHeader(HttpHeaderNames::LOCATION, *this->_configs->getReturn().getPath());
			this->sendResponseRootSlice();
			return ;
		}

		HttpResponse::configureStatusLine();
		HttpResponse::configureHeaders();

		if (HttpStatusCode::isErrorStatusCode(this->_statusCode)) {
			this->_requestedFile.close();
			std::map<int, std::string> errorPages = this->_configs->getError_page();
			std::map<int, std::string>::iterator errorPageIt = errorPages.find(this->_statusCode);

			if (errorPageIt != errorPages.end()) {
				this->_requestedFile.open(errorPageIt->second);
				if (this->_requestedFile.is_open()) {
					this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::intToString(Util::getFileSize(this->_requestedFile)));
					this->sendResponseRootSlice();
					this->sendBody();
					return ;
				}
			}
			this->sendFailedRequest();
		} else {
			this->sendResponseRootSlice();
			this->sendBody();
		}
	} catch (std::exception &reason) {
		::logRequest(LOGGER_ERROR, reason.what());
	}

}

void HttpResponse::sendResponseRootSlice(void) {
	std::string response = "";
	const std::string *contentLength = this->_headers.getHeader(HttpHeaderNames::CONTENT_LENGTH);

	if (contentLength) {
		unsigned long maxBodySize = this->_configs->getClient_max_body_size();
		unsigned long currentBodySize = std::strtol(contentLength->c_str(), NULL, 0);

		if (this->_statusCode != HttpStatusCode::CONTENT_TOO_LARGE
			&& currentBodySize > maxBodySize) {
			this->_statusCode = HttpStatusCode::CONTENT_TOO_LARGE;
			this->sendFailedRequest();
			ExceptionHandler::OutOfMaxBodyRange();
		}
	}

	response += RootConfigs::SupportedHttpProtocol + " ";
	response += Util::intToString(this->_statusCode) + " ";
	if (this->_isReturnTerminatedResponse
		&& !HttpStatusCode::isRedirectStatusCode(this->_statusCode)
		&& this->_configs->getReturn().getPath()) {
		response += *this->_configs->getReturn().getPath() + "\r\n";
	} else {
		response += HttpStatusCode::getStatusCode(this->_statusCode) + "\r\n";
	}

	::logRequest(HttpStatusCode::isErrorStatusCode(this->_statusCode) ? LOGGER_ERROR : LOGGER_INFO, "Responding to client " + Util::intToString(this->_writeSocketFd) + " with -> " + Util::intToString(this->_statusCode) + " " + HttpStatusCode::getStatusCode(this->_statusCode));
	response += this->_headers.toString() + "\r\n";

	send(this->_writeSocketFd, response.c_str(), response.length() * sizeof(char), 0);

}

void HttpResponse::configureStatusLine(void) {
	int16_t statusCode = HttpStatusCode::INVALID_STATUS_CODE;
	ActiveMetods activeMethods = this->_configs->getAllow_methods();
	std::string statusLineStr = "";
	std::stringstream ss;

	if (!activeMethods.isAvailableMethod(this->_request->getMethod())) {
		this->_statusCode = HttpStatusCode::NOT_ALLOWED;
		return ;
	}

	if (this->_requestedFile.is_open()) {

		if (Util::isDirectory(this->_request->getFullFilePath().c_str())) {
			std::vector<std::string> indexFiles = this->_configs->getIndex();
			std::vector<std::string>::iterator indexFilesIt = indexFiles.begin();

			this->_requestedFile.close();
			while (indexFilesIt != indexFiles.end()) {
				this->_requestedFile.open(this->_request->getFullFilePath() + (*indexFilesIt));
				if (this->_requestedFile.is_open()) {
					statusCode = HttpStatusCode::OK;
					this->_requestedFilePath = *indexFilesIt;
					break ;
				}
				indexFilesIt++;
			}

			if (statusCode == HttpStatusCode::INVALID_STATUS_CODE) {
				if (this->_configs->getAutoindex()) {
					try {
						this->_folderStructure = new std::string(AutoIndexController::CreatHtmlFile(this->_request->getFullFilePath(), this->_request->getEndpoint()));
						statusCode = HttpStatusCode::OK;
					} catch (std::exception & reason) {
						statusCode = HttpStatusCode::FORBIDDEN;
					}
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

	this->_statusCode = statusCode;
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

	std::string cookieHeader = this->_request->getHeader(HttpHeaderNames::COOKIE);
	if (cookieHeader.length()) {
		this->_headers.setHeader(HttpHeaderNames::COOKIE, cookieHeader);
	}
}

void HttpResponse::configureHeaders() {
	this->configureDefaultHeaders();

	if (this->_requestedFile.is_open()) {

		std::stringstream ss;
		std::string key, value;
		size_t fileLength = Util::getFileSize(this->_requestedFile);

		ss << fileLength;
		std::getline(ss, value);
		this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, value);
	} else if (this->_folderStructure) {
		this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::intToString(this->_folderStructure->length()));
	}

}

void HttpResponse::sendBody() {
	if (this->_folderStructure) {
		send(this->_writeSocketFd, this->_folderStructure->c_str(), this->_folderStructure->length() * sizeof(char), 0);
		return ;
	}

	std::string line;
	std::streamsize bytesRead;
	char buffer[1024];
	size_t length = 0;

	(void)length;
	(void)this->_request;
	bzero(&buffer, sizeof(buffer));
	while ((bytesRead = this->_requestedFile.read(buffer, sizeof(buffer)).gcount()) > 0) {
		length += strlen(buffer);
		send(this->_writeSocketFd, &buffer, sizeof(buffer), 0);
		bzero(&buffer, sizeof(buffer));
	}
}

void HttpResponse::sendFailedRequest(void) {
	std::string errorPageHtml = Util::generateDefaultErrorPage(this->_statusCode) + "\r\n\r\n";

	this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::intToString(errorPageHtml.length()));
	this->sendResponseRootSlice();
	send(this->_writeSocketFd, errorPageHtml.c_str(), errorPageHtml.length() * sizeof(char), 0);
}
