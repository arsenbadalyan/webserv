#ifndef __HTTP_STATUS_CODE__HPP
#define __HTTP_STATUS_CODE__HPP

#include <iostream>
#include <map>

struct HttpStatusCode {

    static const int16_t OK = 200;
    static const int16_t CREATED = 201;
    static const int16_t NO_CONTENT = 204;
    static const int16_t BAD_REQUEST = 400;
    static const int16_t UNAUTHORIZED = 401;
    static const int16_t FORBIDDEN = 403;
    static const int16_t NOT_FOUND = 404;
    static const int16_t INTERNAL_SERVER_ERROR = 500;
    static const int16_t SERVICE_UNAVAILABLE = 503;

    private:
        HttpStatusCode(void);
        HttpStatusCode(const HttpStatusCode &);
        ~HttpStatusCode();
        void operator=(const HttpStatusCode &);

};

#endif // !__HTTP_STATUS_CODE__HPP