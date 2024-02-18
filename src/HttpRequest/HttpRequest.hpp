#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__

#include <sys/socket.h>
#include "HttpHeaders.hpp"

#define BUFFER_SIZE 1024
#define TERMINATION_BUFFER "\r\n\r\n"

class HttpRequest {

    public:
        HttpRequest(int fd);
        ~HttpRequest();

    public:
        std::string readSocket(int fd);
        void requestParser();

    private:
        HttpRequest(void);
        HttpRequest& operator=(const HttpRequest &);
        HttpRequest(const HttpRequest &);

    private:
        HttpHeaders headers;
        std::string body;
};

#endif // !__HTTP_REQUEST__