#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) {}
HttpResponse::HttpResponse(const HttpResponse&) {}
HttpResponse::~HttpResponse() {}
void HttpResponse::operator=(const HttpResponse&) {}

void HttpResponse::getResponse(const HttpRequest & request, int socketFd) {
	std::string filename = "." + request.getEndpoint();
	std::ifstream requestedFile(filename);
	std::string response = "";

	response += HttpResponse::configureStatusLine(request);
	response += HttpResponse::configureHeaders(request, requestedFile);

	send(socketFd, response.c_str(), response.length() * sizeof(char), 0);
	HttpResponse::sendBody(request, requestedFile, socketFd);
	requestedFile.close();
}

std::string HttpResponse::configureStatusLine(const HttpRequest & request) {
	std::string statusLineStr = "";
	std::stringstream ss;

	ss << HttpStatusCode::OK;
	statusLineStr += RootConfigs::SupportedHttpProtocol + " ";
	statusLineStr += ss.str() + " ";
	statusLineStr += HttpStatusCode::getStatusCode(HttpStatusCode::OK) + "\r\n";

	return (statusLineStr);
}

std::string HttpResponse::configureHeaders(const HttpRequest & request, std::ifstream & ifs) {
	HttpHeaders headers;
	std::string headersStr = "";

	if (ifs.is_open()) {
		std::stringstream ss;
		std::string key, value;
		size_t fileLength = Util::getFileSize(ifs);

		ss << fileLength;
		std::getline(ss, value);
		key = std::string("Content-Length");
		headers.setHeader(key, value);
	}

	headersStr += headers.toString();
	headersStr += "\r\n";

	return (headersStr);
}

void HttpResponse::sendBody(const HttpRequest & request, std::ifstream & ifs, int socketFd) {
	std::string line;
	std::streamsize bytesRead;
	char buffer[1024];
	size_t len = 0;

	bzero(&buffer, sizeof(buffer));
	while ((bytesRead = ifs.read(buffer, sizeof(buffer)).gcount()) > 0) {
		len += strlen(buffer);
		send(socketFd, &buffer, sizeof(buffer), 0);
		bzero(&buffer, sizeof(buffer));
	}
}
