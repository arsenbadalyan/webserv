#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include <sys/socket.h>
#include <sstream>
#include "HttpHeaders.hpp"
#include "Util.hpp"
#include "RootConfigs.hpp"

#define BUFFER_SIZE 1024
#define TERMINATION_BUFFER "\r\n\r\n"
#define SUPPORTED_PROTOCOL ""

#define ERR_INVALID_REQUEST "request startline is invalid"
#define ERR_NOT_SUPPORTED_HTTP_METHOD "http method is not supported"
#define ERR_NOT_SUPPORTED_HTTP_PROTOCOL "http protocol is not supported"

class HttpRequest {

	public:
		HttpRequest(int fd);
		~HttpRequest();

	public:
		std::string readSocket(int fd) const;
		void requestStartLineParser(std::string &);

	private:
		HttpRequest(void);
		HttpRequest& operator=(const HttpRequest &);
		HttpRequest(const HttpRequest &);

	private:
		HttpHeaders headers;
		std::string method;
		std::string endpoint;
		std::string body;

};

#endif // !__HTTP_REQUEST__