#ifndef __FOR_AUTO_INDEX__
#define __FOR_AUTO_INDEX__

#include <list>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include "Util.hpp"
#include "ReadFile.hpp"
#include "Config.hpp"
#include "DirStruct.hpp"
#include "ExceptionHandler.hpp"

class ForAutoIndex {
	public:
		static	std::string CreatHtmlFile(std::string rootHref, std::string endpoint);

	private:
		ForAutoIndex(){};
		static	std::list<DirStruct>	getDirStruct(const std::string & rootHref, const std::string & endpoint);
		static std::string				CreateStringFromFile(std::list<DirStruct> &ls, std::string root);
};



#endif