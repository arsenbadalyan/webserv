#include <ForAutoIndex.hpp>

std::string ForAutoIndex::CreatHtmlFile(std::string rootHref, std::string endpoint)
{
	std::list<DirStruct> str = ForAutoIndex::getDirStruct(rootHref, endpoint);
	return(CreateStringFromFile(str, rootHref));
}

std::list<DirStruct> ForAutoIndex::getDirStruct(const std::string & rootHref, const std::string & endpoint)
{
	std::list<DirStruct> res;
	struct dirent *dr;
	DIR *fi = opendir(rootHref.c_str());

	if (!fi)
		ExceptionHandler::FailedToReadDir();

	while(1) {
		dr = readdir(fi);
		if (!dr)
			break;
		if (std::string(dr->d_name) == ".")
			continue;
		DirStruct ds;
		ds.setName(dr->d_name);
		if (ds.getName() != "..") {
			ds.setHref(Util::removeAddnSlashes(endpoint + "/" + dr->d_name));
			struct stat info;

			if (stat(std::string(Util::removeAddnSlashes(rootHref + "/" + dr->d_name)).c_str(), &info)  != 0) 
				ExceptionHandler::FailedToReadDir();
			else {
				ds.setSize(std::to_string(info.st_size));
				std::tm * ptm = std::localtime(&info.st_mtimespec.tv_sec);
				char buffer[32];
				std::strftime(buffer, 32, "%d.%m.%Y %H:%M", ptm);
				ds.setData(std::ctime(&info.st_mtimespec.tv_sec));
				(void)ds.getData().pop_back();
			}
		} else {
			ds.setHref("../");
		}
		res.push_back(ds);
	}
	return(res);
}

std::string	ForAutoIndex::CreateStringFromFile(std::list<DirStruct> & ls, std::string root)
{
	// ReadFile rdFi("temp/start");
	std::string res(ForAutoIndex::getStart());
	res += root;
	// rdFi.setFilename("temp/first");
	res += ForAutoIndex::getFirst();
	res += root;
	// rdFi.setFilename("temp/second");
	res += ForAutoIndex::getSecond();
	// rdFi.setFilename("temp/end");
	// std::string	end(rdFi.Read());

	for (std::list<DirStruct>::iterator it = ls.begin(); it != ls.end(); ++it)
	{
		res += std::string("    <li>\n      <a href=\"");
		// res += root + std::string("//") ;
		res += it->getHref();
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
	res += ForAutoIndex::getEnd();
	return (res);
}

std::string ForAutoIndex::getStart()
{
	return std::string("<!DOCTYPE html>\n\
<html lang=\"ru\">\n\
<head>\n\
  <meta charset=\"UTF-8\">\n\
  <title>Index of ");
}

std::string ForAutoIndex::getFirst()
{
	return std::string("</title>\n\
  <style>\n\
    hr {\n\
      border: 0;\n\
      border-top: 1px solid #ddd;\n\
      border-bottom: 1px solid #ddd;\n\
      margin: 10px 0;\n\
    }\n\
    ul {\n\
      list-style: none;\n\
      padding: 0;\n\
    }\n\
    li {\n\
      display: flex;\n\
      align-items: center;\n\
      margin-bottom: 5px;\n\
    }\n\
    .filename {\n\
      flex: 1;\n\
    }\n\
    .date {\n\
      margin: 0 100px;\n\
    }\n\
    .size {\n\
      width: 50px;\n\
      text-align: right;\n\
    }\n\
  </style>\n\
</head>\n\
<body>\n\
  <h1>Index of ");
}

std::string ForAutoIndex::getSecond()
{
	return std::string("</h1>\n\
  <hr>\n\
  <ul>\n");
}

std::string ForAutoIndex::getEnd()
{
	return std::string("</ul>\n\
<hr>\n\
</body>\n\
</html>\n\n");
}
