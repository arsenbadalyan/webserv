#include "HttpResponse.hpp"

HttpResponse::HttpResponse(const HttpResponse&) {}
void HttpResponse::operator=(const HttpResponse&) {}

HttpResponse::HttpResponse(const HttpRequest * request, int writeSocketFd)
	: _server(request->getServer()),
	_request(request),
	_writeSocketFd(writeSocketFd),
	_requestedFile(request->getFullFilePath())
{
	this->_configs = this->_server->findLocations(request->getEndpoint());
	this->_isReturnTerminatedResponse = !!this->_configs->getReturn().getStatusTypes(); // TODO: refactor after merge
}

HttpResponse::~HttpResponse() {
	this->_requestedFile.close();
}

void HttpResponse::getResponse(void) {
	
	std::string response = "";

	response += HttpResponse::configureStatusLine();
	response += HttpResponse::configureHeaders();

	send(this->_writeSocketFd, response.c_str(), response.length() * sizeof(char), 0);
	HttpResponse::sendBody();

}

std::string HttpResponse::configureStatusLine(void) {
	std::string statusLineStr = "";
	std::stringstream ss;

	ss << HttpStatusCode::OK;
	statusLineStr += RootConfigs::SupportedHttpProtocol + " ";
	statusLineStr += ss.str() + " ";
	statusLineStr += HttpStatusCode::getStatusCode(HttpStatusCode::OK) + "\r\n";

	return (statusLineStr);
}

void HttpResponse::configureDefaultHeaders(void) {
	this->_headers.setHeader(HttpHeaderNames::CONTENT_LENGTH, std::string("0"));
	//  TODO: change keys after merge
	this->_headers.setHeader(HttpHeaderNames::SERVER, std::string(RootConfigs::SERVER_NAME));
	this->_headers.setHeader(HttpHeaderNames::DATE, std::string(""));
	this->_headers.setHeader(HttpHeaderNames::EXPIRES, std::string(""));
	this->_headers.setHeader(HttpHeaderNames::LAST_MODIFIED, std::string(""));
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
	bzero(&buffer, sizeof(buffer));
	while ((bytesRead = this->_requestedFile.read(buffer, sizeof(buffer)).gcount()) > 0) {
		length += strlen(buffer);
		send(this->_writeSocketFd, &buffer, sizeof(buffer), 0);
		bzero(&buffer, sizeof(buffer));
	}
}
