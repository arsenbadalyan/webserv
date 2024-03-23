#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) {}
HttpResponse::HttpResponse(const HttpResponse&) {}
HttpResponse::~HttpResponse() {}
void HttpResponse::operator=(const HttpResponse&) {}

HttpResponse::HttpResponse(const HttpRequest& request) :
	__responseTxt("")
{
	this->configureStatusLine()
		->configureHeaders()
		->configureBody()
		->makeResponse();

}

HttpResponse* HttpResponse::configureStatusLine(void) {
	return (this);
}

HttpResponse* HttpResponse::configureHeaders(void) {
	return (this);
}

HttpResponse* HttpResponse::configureBody(void) {
	return (this);
}
