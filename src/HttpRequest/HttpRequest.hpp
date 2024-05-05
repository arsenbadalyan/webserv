#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include <sys/socket.h>
#include <sstream>
#include <istream>
#include "HttpHeaders.hpp"
#include "Util.hpp"
#include "RootConfigs.hpp"
#include "Server.hpp"

#define READ_BUFFER_SIZE 1024
#define TERMINATION_BUFFER "\r\n\r\n"
#define SUPPORTED_PROTOCOL ""

#define ERR_INVALID_REQUEST "request startline is invalid"
#define ERR_NOT_SUPPORTED_HTTP_METHOD "http method is not supported"
#define ERR_NOT_SUPPORTED_HTTP_PROTOCOL "http protocol is not supported"

struct chunk_type {
	static const size_t no_chunks = 0;
	static const size_t dataForm_chunk = 1;
	static const size_t encoding_chunk = 2;
};

class HttpRequest {

	public:
		HttpRequest(Server* currentServer, int readSocketFd);
		~HttpRequest();

	public:
		std::string requestInitialParsing(int fd);
		void parseHeadersBuffer(std::string &);
		void requestStartLineParser(std::string &);
		void configureRequestByHeaders(void);
		void parseMultipartDataForm(int socket);
		void parseChunkedData(int socket);

	public:
		Server * getServer(void) const;

	public:
		bool hasFinishedReceivingRequest(void) const;
		const std::string getEndpoint(void) const;
		const std::string getFullFilePath(void) const;
		const std::string getMethod(void) const;
		std::string getHeader(std::string headerName) const;

	private:
		HttpRequest(void);
		HttpRequest& operator=(const HttpRequest &);
		HttpRequest(const HttpRequest &);

	private:
		HttpHeaders headers;
		std::string method;
		std::string endpoint;
		std::string body;
		std::string boundary;
		std::string contentType;
		size_t chunking;
		bool _hasFinishedRead;
		Server *_server;

};

#endif // !__HTTP_REQUEST__