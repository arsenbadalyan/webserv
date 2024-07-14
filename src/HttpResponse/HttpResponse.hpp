#ifndef __HTTP_REQUEST__HPP__
#define __HTTP_REQUEST__HPP__

#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <cstdio>
#include "AutoIndexController.hpp"
#include "ActiveMetods.hpp"
#include "ExceptionHandler.hpp"
#include "HttpRequest.hpp"
#include "RootConfigs.hpp"
#include "HttpStatusCode.hpp"
#include "Util.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "HttpHeaderNames.hpp"
#include "Logger.hpp"
#include "CgiHandler.hpp"

class HttpResponse {

	public:
		HttpResponse(const HttpRequest* request, int writeSocketFd);
		~HttpResponse();

	public:
		void getResponse(void);

	public:
		static std::string URLFragmentCutter(const std::string &);

	private:
		void configureStatusLine(void);
		void configureHeaders(void);
		void configureDefaultHeaders(void);
		void sendBody(void);
		void sendFailedRequest(void);
		void sendResponseRootSlice(void);
		void sendResponse(void);
	
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
		std::string _requestedFileType;
		std::string _requestedFilePath;
		int16_t _statusCode;
		std::string *_folderStructure;
		CGIAnswerPair _cgiAnswerPair;
		std::string _responseResult;
};

#endif // !__HTTP_REQUEST__HPP__