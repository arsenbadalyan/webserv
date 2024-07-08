#ifndef __FOR_AUTO_INDEX__
#define __FOR_AUTO_INDEX__

#include <list>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include "Util.hpp"
#include "FileReader.hpp"
#include "Config.hpp"
#include "DirController.hpp"
#include "ExceptionHandler.hpp"

class AutoIndexController {
	public:
		static	std::string CreatHtmlFile(std::string rootHref, std::string endpoint);

	private:
		AutoIndexController(){};
		static std::list<DirController>	getDirController(const std::string & rootHref, const std::string & endpoint);
		static std::string			CreateStringFromFile(std::list<DirController> &ls, std::string root);
		static std::string			getStart();
		static std::string			getFirst();
		static std::string			getSecond();
		static std::string			getEnd();
};



#endif