#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const HttpResponse&) {}
void HttpResponse::operator=(const HttpResponse&) {}

HttpResponse::HttpResponse(const HttpRequest * request, int writeSocketFd)
	: _server(request->getServer()),
	_request(request),
	_writeSocketFd(writeSocketFd),
	_requestedFile(HttpResponse::URLFragmentCutter(request->getFullFilePath()), std::ios::binary),
	_statusCode(HttpStatusCode::INVALID_STATUS_CODE),
	_folderStructure(NULL),
	_cgiAnswerPair(false, ""),
	_responseResult(""),
	_alreadySentBytes(0)
{
	::logRequest(LOGGER_INFO,
		"Sending response -> Client: "
		+ Util::intToString(writeSocketFd) + ", "
		+ "Full path: "
		+ request->getFullFilePath() + ", "
		+ "Endpoint: "
		+ request->getEndpoint());

	this->_requestedFilePath = HttpResponse::URLFragmentCutter(request->getFullFilePath());
	this->_configs = this->_server->findLocations(HttpResponse::URLFragmentCutter(request->getEndpoint()));
	this->_isReturnTerminatedResponse = !(this->_configs->getReturn().getStatusTypes() == NO_CUSTOM_STATUS_CODE);
}

HttpResponse::~HttpResponse() {
	this->_requestedFile.close();
	if (this->_folderStructure) {
		delete this->_folderStructure;
	}
}

bool HttpResponse::getResponse(void) {

	try {

		if (this->_request->isFileUpload()) {
			try {
				HttpResponse::configureDefaultHeaders();
				this->_statusCode = HttpStatusCode::CREATED;
				const std::string* contentDisposition = this->_request->getChunkedDataHeaders().getHeader(HttpHeaderNames::CONTENT_DISPOSITION);

				if (contentDisposition) {
					std::map<std::string, std::string> UploadFileData = HttpHeaders::getContentDispositionData(*contentDisposition);

					std::string fileName = (*UploadFileData.find("filename")).second;
					std::ofstream uploadedFile(this->_server->getServerConfig().getUpload_dir() + "/" + fileName);

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
			return (this->sendResponse());
		}

		if (this->_isReturnTerminatedResponse) {
			this->_statusCode = this->_configs->getReturn().getStatusTypes();
			HttpResponse::configureDefaultHeaders();
			if (HttpStatusCode::isRedirectStatusCode(this->_statusCode)
				&& this->_configs->getReturn().getPath())
				this->_headers.setHeader(HttpHeaderNames::LOCATION, *this->_configs->getReturn().getPath());
			this->sendResponseRootSlice();
			return (this->sendResponse());
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
					return (this->sendResponse());
				}
			}
			this->sendFailedRequest();
		} else if (Util::toLower(this->_request->getMethod()) == SupportedMethods::DELETE) {
			const char* filename = this->_request->getFullFilePath().c_str();

			if (!remove(filename)) {
				this->_statusCode = HttpStatusCode::NO_CONTENT;
			} else {
				this->_statusCode = HttpStatusCode::INTERNAL_SERVER_ERROR;
			}
			this->sendResponseRootSlice();
		} else {
			this->sendResponseRootSlice();
			this->sendBody();
		}
	} catch (std::exception &reason) {
		::logRequest(LOGGER_ERROR, reason.what());
	}

	return (this->sendResponse());
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

	this->_responseResult.append(response.c_str(), response.length() * sizeof(char));
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
			try {
				statusCode = HttpStatusCode::OK;
				this->_cgiAnswerPair = CgiHandler::executeCGI(this->_request->getFullFilePath(), this->_server->getCgiSet());
			} catch(std::exception & reason) {
				statusCode = HttpStatusCode::INTERNAL_SERVER_ERROR;
			}
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

	if (this->_cgiAnswerPair.first) {
		this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::sizeTToString(this->_cgiAnswerPair.second.length()));
	} else if (this->_requestedFile.is_open()) {

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
		std::string folderStructureStr = *this->_folderStructure;
		// send(this->_writeSocketFd, this->_folderStructure->c_str(), this->_folderStructure->length() * sizeof(char), 0);
		this->_responseResult.append(folderStructureStr.c_str(), folderStructureStr.length() * sizeof(char));
		return ;
	}

	if (this->_cgiAnswerPair.first) {
		// send(this->_writeSocketFd, this->_cgiAnswerPair.second.c_str(), this->_cgiAnswerPair.second.length() * sizeof(char), 0);
		this->_responseResult.append(this->_cgiAnswerPair.second.c_str(), this->_cgiAnswerPair.second.length() * sizeof(char));
		return ;
	}

	std::string line;
	std::streamsize bytesRead;
	char buffer[1024];

	bzero(&buffer, sizeof(buffer));
	while ((bytesRead = this->_requestedFile.read(buffer, sizeof(buffer)).gcount()) > 0) {
		this->_responseResult.append(buffer, bytesRead);
		bzero(&buffer, sizeof(buffer));
	}
}

void HttpResponse::sendFailedRequest(void) {
	std::string errorPageHtml = Util::generateDefaultErrorPage(this->_statusCode) + "\r\n\r\n";

	this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, Util::intToString(errorPageHtml.length()));
	this->sendResponseRootSlice();
	// send(this->_writeSocketFd, errorPageHtml.c_str(), errorPageHtml.length() * sizeof(char), 0);
	this->_responseResult.append(errorPageHtml.c_str(), errorPageHtml.length() * sizeof(char));
}


// Static Methods
std::string HttpResponse::URLFragmentCutter(const std::string& URL) {

	std::string finalResultStr = URL;
	size_t fragmentSymbolPos = std::string::npos;
	std::string fragmentSymbols = "?#";
	size_t i = fragmentSymbols.length();

	while (i--) {
		fragmentSymbolPos = finalResultStr.find(fragmentSymbols[i]);

		if (fragmentSymbolPos != std::string::npos)
			finalResultStr = finalResultStr.substr(0, fragmentSymbolPos);
	}

	return (finalResultStr);
	
}

bool HttpResponse::sendResponse(void) {
	std::string toBeSent = this->_responseResult.substr(this->_alreadySentBytes, std::string::npos);
	int result = write(this->_writeSocketFd, toBeSent.c_str(), toBeSent.length() * sizeof(char));

	if (result == -1) {
		::logRequest(LOGGER_ERROR, "Something went wrong when sending request to client -> " + Util::intToString(this->_writeSocketFd));
		return (true);
	} else {
		this->_alreadySentBytes += result;
	}

	if (this->_alreadySentBytes != this->_responseResult.length()) {
		return (false);
	}

	return (true);
}