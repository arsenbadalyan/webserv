#ifndef __HTTP_HEADERS__HPP
#define __HTTP_HEADERS__HPP

#include <iostream>
#include <map>
#include <cstring>
#include "ExceptionHandler.hpp"
#include "Util.hpp"
#include "RootConfigs.hpp"

typedef std::map<std::string, std::string> HeadersMap;

struct HttpHeaders {

	public:
        HttpHeaders(void);
		~HttpHeaders();

	public:
		const std::string* getHeader(const std::string key) const;
		HttpHeaders& setHeader(std::string& key, std::string& value);
		HttpHeaders& setHeader(std::string& line);

	private:
		HttpHeaders(const HttpHeaders &);
		void operator=(const HttpHeaders &);

	private:
		void headerValidator(std::string& key, std::string& value) const;

	private:
		HeadersMap headers;

};

#endif // !__HTTP_HEADERS__HPP