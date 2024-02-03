#ifndef _CGIHANDLER_HPP_
#define _CGIHANDLER_HPP_

#include <iostream>

class CgiHandler
{
private:
	std::string _php;
	std::string _python;
public:
	CgiHandler();
	CgiHandler(std::string php, std::string python);
	CgiHandler(const CgiHandler& other);
	CgiHandler& operator= (const CgiHandler& other);
	~CgiHandler();
	std::string getPhp() const;
	std::string getPython() const;
	CgiHandler& setPhp(const std::string php);
	CgiHandler& setPython(const std::string python);
};

#endif