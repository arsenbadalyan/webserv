#ifndef __REQUEST_POOL__
#define __REQUEST_POOL__

#include <iostream>
#include <map>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"
#include "ExceptionHandler.hpp"

typedef std::map<int, HttpRequest*> RequestPoolMap;

class RequestPool {

    public:
        RequestPool(void);
        ~RequestPool();

    public:
        bool sendRequest(int clientSocket, Server & server); // --- if request has finished returns false, otherwise true
        void getResponse(int clientSocket);
        void destroyRequest(int clientSocket);

    private:
        RequestPool(const RequestPool &);
        RequestPool& operator=(const RequestPool &);

    public:
        Server *server;
        RequestPoolMap requestPool;
};

#endif // !__REQUEST_POOL__