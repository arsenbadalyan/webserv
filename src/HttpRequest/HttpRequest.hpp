#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include <sys/socket.h>
#include <sstream>
#include "HttpHeaders.hpp"

#define BUFFER_SIZE 1024
#define TERMINATION_BUFFER "\r\n\r\n"
#define SUPPORTED_PROTOCOL "HTTP/1.1"

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