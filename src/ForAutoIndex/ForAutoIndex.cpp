#include <ForAutoIndex.hpp>

std::string ForAutoIndex::ChreatHtmlFile(const Config & loc)
{
	std::list<DirStruct> str = ForAutoIndex::getDirStruct(loc.getRoot());
	return(Chreatstring(str, loc.getRoot().c_str()));
}

std::list<DirStruct> ForAutoIndex::getDirStruct(std::string root)
{
	std::list<DirStruct> res;
	struct dirent *dr;
	DIR *fi = opendir(root.c_str());
	if (!fi)
		throw MyException("Nod open root for creat Html file for autoindex");
	for(; 1; )
	{
		dr = readdir(fi);
		if (!dr)
			break;
		if (std::string(dr->d_name) == ".")
			continue;
		DirStruct ds;
		ds.setName(dr->d_name);
		if (ds.getName() != "..")
		{
			struct stat info;
			if (stat(std::string(root + "/" + dr->d_name).c_str(), &info)  != 0) 
				throw MyException("Nod read root for creat Html file for autoindex");
			else {
				ds.setSize(std::to_string(info.st_size));
				std::tm * ptm = std::localtime(&info.st_mtimespec.tv_sec);
				char buffer[32];
				std::strftime(buffer, 32, "%d.%m.%Y %H:%M", ptm);
				ds.setData(std::ctime(&info.st_mtimespec.tv_sec));
				(void)ds.getData().pop_back();
			}
		}
		res.push_back(ds);
	}
	return(res);
}

std::string	ForAutoIndex::Chreatstring(std::list<DirStruct> & ls, std::string root)
{
	ReadFile rdFi("temp/start");
	std::string res(rdFi.Read());
	res += root;
	rdFi.setFilename("temp/first");
	res += rdFi.Read();
	res += root;
	rdFi.setFilename("temp/second");
	res += rdFi.Read();
	rdFi.setFilename("temp/end");
	std::string	end(rdFi.Read());

	for (std::list<DirStruct>::iterator it = ls.begin(); it != ls.end(); ++it)
	{
		res += std::string("    <li>\n      <a href=\"");
		// res += root + std::string("//") ;
		res += it->getName();
		res += std::string("\" class=\"filename\">");
		res += it->getName();
		res += std::string("</a>\n");
		if (it->getName() != ".")
		{
			res += std::string("      <span class=\"date\">");
			res += it->getData();
			res += std::string("</span>\n      <span class=\"size\">");
			res += it->getSize();
			res += std::string("</span>\n");
		}
		res += std::string("    </li>\n");
	}
	res += end;
	return (res);
}



ForAutoIndex::MyException::MyException(const std::string &error) : _error(error) {}

ForAutoIndex::MyException::~MyException() throw() {}

const char * ForAutoIndex::MyException::what() const throw() { return ((this->_error.data())); }