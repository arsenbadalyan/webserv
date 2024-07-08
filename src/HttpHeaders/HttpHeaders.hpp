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
		HttpHeaders& setHeader(const std::string key, const std::string value);
		HttpHeaders& setHeader(std::string& line);
		std::string toString(void);

	public:
		static std::map<std::string, std::string> getContentDispositionData(std::string input); // throws exception in case of parse failure

	private:
		HttpHeaders(const HttpHeaders &);
		void operator=(const HttpHeaders &);

	private:
		void headerValidator(const std::string& key, const std::string& value) const;
		static std::string* parseDispositionDataLinePart(std::string haystack, std::string needle);

	private:
		HeadersMap headers;

};

#endif // !__HTTP_HEADERS__HPP