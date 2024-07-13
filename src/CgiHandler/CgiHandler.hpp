#ifndef _CGIHANDLER_HPP_
#define _CGIHANDLER_HPP_

#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include <map>

typedef std::pair<bool, std::string> CGIAnswerPair;

class CgiHandler
{
private:
	CgiHandler();
	CgiHandler(const CgiHandler& other);
	CgiHandler& operator= (const CgiHandler& other);
	~CgiHandler();

private:
	static void cp(char* cp, std::string a);

public:
	static CGIAnswerPair executeCGI(std::string file, std::map<std::string, std::string> &ma);

};

#endif