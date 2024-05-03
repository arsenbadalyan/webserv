#ifndef __FOR_AUTO_INDEX__
#define __FOR_AUTO_INDEX__

#include "Config.hpp"
#include "DirStruct.hpp"
#include <list>
#include <dirent.h>
// #include <sys/types.h>
#include <sys/stat.h>
#include "ReadFile.hpp"
#include <ctime>

class ForAutoIndex
{
public:
	static	std::string ChreatHtmlFile(const Config & loc);
private:
	ForAutoIndex(){};
	static	std::list<DirStruct>	getDirStruct(const std::string root);
	static std::string				Chreatstring(std::list<DirStruct> &ls, std::string root);

	class MyException : public std::exception
	{
	private:
		std::string _error;
	public:
		MyException(const std::string & error);
		~MyException() throw();

		const char * what() const throw();
	};
};



#endif