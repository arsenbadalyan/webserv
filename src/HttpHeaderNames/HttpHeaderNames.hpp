#ifndef __HTTP_HEADER_NAMES_HPP__
#define __HTTP_HEADER_NAMES_HPP__

#include <iostream>

class HttpHeaderNames {

	public:
		static const std::string CONTENT_TYPE;
		static const std::string CONTENT_LENGTH;
		static const std::string DATE;
		static const std::string SERVER;
		static const std::string EXPIRES;
		static const std::string LAST_MODIFIED;
		static const std::string LOCATION;
		static const std::string CONNECTION;
		static const std::string COOKIE;
		static const std::string TRANSFER_ENCODING;

	private:
		HttpHeaderNames(void);
		HttpHeaderNames(const HttpHeaderNames & target);
		void operator=(const HttpHeaderNames & rhs); 
		~HttpHeaderNames();

};

#endif // !__HTTP_HEADER_NAMES_HPP__