#ifndef __FOR_AUTO_INDEX__
#define __FOR_AUTO_INDEX__

#include "Config.hpp"
#include "DirStruct.hpp"
#include <list>

class ForAutoIndex
{
public:
	static	std::string ChreatHtmlFile(const Config & loc);
private:
	ForAutoIndex(){};
	static	std::list<DirStruct>	getDirStruct(const std::string root);
	static std::string				Chreatstring(const std::list<DirStruct> &ls);
};


#endif