#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include <sys/socket.h>
#include <sstream>
#include <istream>
#include <fstream>
#include "HttpHeaders.hpp"
#include "Util.hpp"
#include "RootConfigs.hpp"
#include "Server.hpp"
#include "HttpHeaderNames.hpp"
#include "Logger.hpp"

#define READ_BUFFER_SIZE (1024 * 1024)
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
		HttpRequest& requestInitialParsing(int fd);
		void parseHeadersBuffer(std::string &);
		void requestStartLineParser(std::string &);
		void configureRequestByHeaders(void);
		void parseMultipartDataForm(int socket);
		void parseChunkedData(int socket);
		void prepareRead(int socket);
		HttpRequest& extractBody(int sockFd, std::string initialData);
		HttpRequest& extractChunk(int sockFd, std::string initialData);

	public:
		Server * getServer(void) const;

	public:
		bool hasFinishedReceivingRequest(void) const;
		bool isFileUpload(void) const;
		const HttpHeaders& getChunkedDataHeaders(void) const;
		const std::string getEndpoint(void) const;
		const std::string getFullFilePath(void) const;
		const std::string getMethod(void) const;
		const std::string& getBody(void) const;
		std::string getHeader(std::string headerName) const;

	private:
		bool hasStartBoundary(const std::string& str);
		size_t hasEndBoundary(const std::string& str);
		HttpRequest& makeChunkRegularCheck(void);

	private:
		HttpRequest(void);
		HttpRequest& operator=(const HttpRequest &);
		HttpRequest(const HttpRequest &);

	private:
		HttpHeaders headers;
		HttpHeaders _chunkedDataHeaders;
		std::string method;
		std::string endpoint;
		std::string body;
		std::string boundary;
		std::string contentType;
		size_t contentLength;
		size_t chunking;
		bool _hasFinishedRead;
		Server *_server;
		double receivedBytes;
		bool _hasReadChunkedDataHeaders;
		bool _isUploadedFile;
		int _fd;

};

#endif // !__HTTP_REQUEST__