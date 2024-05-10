#ifndef _CGI_EXEC_HPP_
#define _CGI_EXEC_HPP_

#include <iostream>
#include <string>
#include <unistd.h>

#define PYTHONPATH "./CGI_interpritatrs/python-cgi-mac"

class CgiExec
{
public:
	static void executeCGI(std::string filePath, std::string fileType);
};

#endif