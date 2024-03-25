#ifndef __HTTP_REQUEST__HPP__
#define __HTTP_REQUEST__HPP__

#include <iostream>
#include <fstream>
#include "HttpRequest.hpp"
#include "RootConfigs.hpp"
#include "HttpStatusCode.hpp"
#include "Util.hpp"

class HttpResponse {

    public:
        static void getResponse(const HttpRequest & request, int socketFd);

    private:
        static std::string configureStatusLine(const HttpRequest & request);
        static std::string configureHeaders(const HttpRequest & request, std::ifstream & ifs);
        static void sendBody(const HttpRequest & request, std::ifstream & ifs, int socketFd);
    
    private:
        HttpResponse(void);
        HttpResponse(const HttpResponse&);
        ~HttpResponse();
        void operator=(const HttpResponse&);

    private:
        std::string __responseTxt;
};

#endif // !__HTTP_REQUEST__HPP__