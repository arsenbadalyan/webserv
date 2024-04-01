#include <ForAutoIndex.hpp>

std::string ForAutoIndex::ChreatHtmlFile(const Config & loc)
{
	std::list<DirStruct> str = ForAutoIndex::getDirStruct(loc.getRoot());
	return(Chreatstring(str));
}

std::list<DirStruct> ForAutoIndex::getDirStruct(std::string root)
{
	return(std::list<DirStruct>());
}

std::string	ForAutoIndex::Chreatstring(const std::list<DirStruct> & ls)
{
	return (std::string());
}