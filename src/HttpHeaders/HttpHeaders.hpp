#ifndef __HTTP_HEADERS__HPP
#define __HTTP_HEADERS__HPP

#include <iostream>
#include <map>

#define INVALID_HEADER_KEY_CHARS "()<>@,;:\\\"/[]?={} "

struct HttpHeaders {

    public:
        const std::string& getHeader( std::string& headerKey ) const;
        HttpHeaders& setHeader( std::string& key, std::string& value );

    private:
        bool headerValidator(std::string& key, std::string& value) const;

    private:
        std::map<std::string, std::string> headers;

};

#endif // !__HTTP_HEADERS__HPP