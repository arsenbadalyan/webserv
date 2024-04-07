#ifndef __HTTP_REQUEST__HPP__
#define __HTTP_REQUEST__HPP__

#include <iostream>
#include <fstream>
#include "HttpRequest.hpp"
#include "RootConfigs.hpp"
#include "HttpStatusCode.hpp"
#include "Util.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "HttpHeaderNames.hpp"

class HttpResponse {

	public:
		HttpResponse(const HttpRequest* request, int writeSocketFd);
		~HttpResponse();

	public:
		void getResponse(void);

	private:
		std::string configureStatusLine();
		std::string configureHeaders();
		void configureDefaultHeaders(void);
		void sendBody();
	
	private:
		HttpResponse(const HttpResponse&);
		void operator=(const HttpResponse&);

	private:
		HttpHeaders _headers;
		Server *_server;
		Config *_configs;
		const HttpRequest *_request;
		int _writeSocketFd;
		bool _isReturnTerminatedResponse;
		std::ifstream _requestedFile;
};

#endif // !__HTTP_REQUEST__HPP__