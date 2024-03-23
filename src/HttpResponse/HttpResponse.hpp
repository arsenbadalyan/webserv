#ifndef __HTTP_REQUEST__HPP__
#define __HTTP_REQUEST__HPP__

#include "HttpRequest.hpp"

class HttpResponse {

    public:
        HttpResponse(const HttpRequest& request);
    
    private:
        HttpResponse* configureStatusLine(void);
        HttpResponse* configureHeaders(void);
        HttpResponse* configureBody(void);
        HttpResponse* makeResponse(void);
        void send(void);
    
    private:
        HttpResponse(void);
        HttpResponse(const HttpResponse&);
        ~HttpResponse();
        void operator=(const HttpResponse&);

    private:
        std::string __responseTxt;
};

#endif // !__HTTP_REQUEST__HPP__