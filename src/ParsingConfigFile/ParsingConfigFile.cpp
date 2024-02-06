#include "ParsingConfigFile.hpp"

ParsingConfigFile::ParsingConfigFile(const std::string & filename) : _file(filename)	{}

ParsingConfigFile::ParsingConfigFile(const ParsingConfigFile & other) 
{
	this->_file = other._file;
	this->_data = other._data;
}

ParsingConfigFile & ParsingConfigFile::operator=(const ParsingConfigFile & other)
{
	if (this != &other)
	{
		this->_file = other._file;
		this->_data = other._data;
	}
	return (*this);
}

ParsingConfigFile::~ParsingConfigFile() { }

bool ParsingConfigFile::startPars()
{
	this->_data = this->_file.Read();
	if (!this->_data.size())
		throw MyException("Emty File ERROR");
	if (!this->CheckSintex())
		throw MyException("Sintexs ERROR");
	if (!this->CheckCorecktServer())
		throw MyException("Corect Error");
	return (true);
}

bool ParsingConfigFile::CheckSintex()
{
	int flag = 0;

	for (size_t i = 0; i < this->_data.size(); i++)
	{
		if(this->_data[i] == '{')
			++flag;
		if(this->_data[i] == '}')
			--flag;
		if (flag < 0)
			throw MyException("Error {");
	}
	if (flag > 0)
		throw MyException("Error }");
	if (flag < 0)
		throw MyException("Error {");
	return (true);
}

bool ParsingConfigFile::CheckCorecktServer()
{
	std::string server = "server";

	// this->_sizeList = 0;
	for (size_t pos_start = 0, pos_end = 0, pos = 0; \
		pos_start < this->_data.size(); pos_start = pos_end + 1)
	{
		pos_end = this->_data.find('{', pos_start);
		if (SIZE_MAX == pos_end && pos_start == 0)
			throw MyException("Suntexs Error");
		if (SIZE_MAX == pos_end)
		{
			for (size_t i = pos_start; i < this->_data.size() ; ++i)
			{
				if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
					throw MyException("Suntexs Error");
			}
			return (true);
		}
		std::transform(this->_data.begin(), this->_data.begin() + (pos_end - pos_start), \
			this->_data.begin(), ::tolower);
		pos = this->_data.find(server.c_str(), pos_start, pos_end - pos_start);
		if (SIZE_MAX == pos_end)
			throw MyException("Suntexs Error don exist server");
		for (size_t i = pos_start; i < pos_end; ++i)
		{
			if (i < pos && i > pos + server.size() && \
				this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
				{
					// std::cout << "i = " << i  << "  " << (int)this->_data[i] << std::endl;
					throw MyException("Suntexs Error");
				}
		}
		pos_start = pos_end + 1;
		pos_end = get_reng(pos_start);
		if (pos_end == SIZE_MAX)
			throw MyException("Suntexs Error");
		if (!CheckCoreckt(pos_start, pos_end, false))
			throw MyException("Suntexs Error");
		if (pos_end == this->_data.size())
			return (true);
	}
	return (true);
}

size_t ParsingConfigFile::get_reng(size_t start)
{
	int flag = 1;
	for ( ; start < this->_data.size(); ++start)
	{
		if (this->_data[start] == '{')
			++flag;
		if (this->_data[start] == '}')
			--flag;
		if (!flag)
			return (start);
	}
	return (SIZE_MAX);
}

/*
	server { lisen : 127.0.0.1:8080  }
   pos_start^                 pos_end^
*/
bool ParsingConfigFile::CheckCoreckt(size_t pos_start, size_t pos_end, bool controlFlag)
{
	// std::string conf_name;
	if (!controlFlag)
		this->_serverList.push_back(Server());
	// else
	// 	this->_serverList[this->_serverList.size() -1].getLocations().push_back(Config());
	std::string cpy;
	for (size_t i = pos_start, pos = 0; i < pos_end;)
	{
		if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ' )
		{
			pos = i;
			for (; this->_data[i] != '\t' && this->_data[i] != ' ' && this->_data[i] != '}'; i++);
			if (this->_data[i] == '}')
				throw MyException("Emty Lokacion");
			cpy =  this->_data.substr(pos, i - pos);
			std::transform(cpy.begin(), cpy.end(), cpy.begin(), ::tolower);
			if (std::string("location") == cpy)
			{
				if (controlFlag)
					throw MyException("Error double location");
				this->_serverList[this->_serverList.size() -1].getLocations().push_back(Config());
				i = CheckCorecktLocation(i, pos_end);
			}
			else
				i = CheckCorecktConfig(cpy, i, pos_end, controlFlag);
			if (SIZE_MAX == i)
				throw MyException("Suntexs Error");
		}
		else
			++i;
		cpy.clear();
	}
	return (true);
}

/*
	server {    location   /test   {    aaa aaa:   }  }
         		        ^pos_start             pos_end^
*/

size_t ParsingConfigFile::CheckCorecktLocation(size_t pos_start, size_t pos_end)
{
	size_t start = pos_start, end = 0;
	for (; start < pos_end;)
	{
		if (this->_data[start] != '\t' && this->_data[start] != ' ')
		{
			if (this->_data[start] == '\n')
				throw MyException("Error Emty name location");
			for (end = start; end < pos_end && this->_data[end] != '\t' && \
				this->_data[end] != '\n' && this->_data[end] != ' ' && this->_data[end] != '{'; ++end);
			//lokaciayi anun
			this->_serverList[this->_serverList.size() -1].getLocations()\
			[this->_serverList[this->_serverList.size() -1].getLocations().size() - 1].\
			setLocation_name(std::string(this->_data.begin() + start, this->_data.begin() + end));
			start = end;
			end = this->_data.find('{', start);
			for (size_t i = start; i < end; ++i)
			{
				if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
				{
					throw MyException("Error name lokacion");
				}
			}
			start = end + 1;
			end = get_reng(start);
			if (CheckCoreckt(start, end, true))
				throw MyException("Error Lokacion");
			return (end + 1);
		}
		else
			++start;
	}
	throw MyException("Error Emty name location");
	return size_t(SIZE_MAX);
}

/*
	server {  lisen           : 127.0.0.1:8080;  }
         config ^  ^pos_start             pos_end^
*/

size_t	ParsingConfigFile::CheckCorecktConfig(std::string config, size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t res = SIZE_MAX;
	if (std::string("listen") == config)
	{
		if (controlFlag)
			throw MyException("Error listen for location");
		res = checkListen(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	} 
	else if (std::string("server_name") == config)
	{
		if (controlFlag)
			throw MyException("Error server_name for location");
		res = checkServerName(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	} 
	else if (std::string("root") == config)
	{
		res = checkRoot(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("allow_methods") == config)
	{
		res = checkAllowMethods(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("upload_dir") == config)
	{
		res = checkUploadDir(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("error_page") == config)
	{
		res = checkErrorPage(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("return") == config)
	{
		res = checkReturn(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("cgi") == config)
	{
		res = checkCgi(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("index") == config)
	{
		res = checkIndex(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("autoindex") == config)
	{
		res = checkAutoindex(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("client_max_body_size") == config)
	{
		res = checkClientMaxBodySize(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	throw MyException("Error no config");
	return (SIZE_MAX);
}


/*
	server {  lisen             127.0.0.1:8080   ;  }
         		   ^pos_start             pos_end^

*/

size_t ParsingConfigFile::checkListen(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0, start = pos_start;
	// this->_data.find(';', pos_start);
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis Listen");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis Listen");
	pos_end = end;
	for (; start < end && (this->_data[start] == '\t' || this->_data[start] != ' '); ++start);
	if (start >= end)
		throw MyException("Error Emty Listen");
	for (end = start; end < pos_end && this->_data[end] != '\t' && this->_data[end] != ' '; ++end);
	for (i = end; i < pos_end; ++i)
		if (this->_data[i] != '\t' && this->_data[i] != ' ')
			throw MyException("Error Sintexts Listen");
	if (!checCorectHostAndPort(start, end, controlFlag))
		throw MyException("Error Sintexts Listen");
	return size_t(pos_end + 1);
}

size_t ParsingConfigFile::checkServerName(size_t pos_start, size_t pos_end, bool controlFlag)
{
	(void)controlFlag;
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis ServerName");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis ServerName");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, check;
	str >> val;
	str >> check;
	if (check.size())
		throw MyException("Error Sintexsis ServerName arguments");
	this->_serverList[this->_serverList.size() - 1].setServerName(val);
	return size_t(end + 1);
}

size_t ParsingConfigFile::checkRoot(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis root");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis root");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, check;
	str >> val;
	str >> check;
	if (check.size())
		throw MyException("Error Sintexsis root arguments");
	DIR *dir = opendir(val.c_str());
	if (dir)
		throw MyException("Error Sintexsis root not direktory");
	closedir(dir);
	if (controlFlag)
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].setRoot(val);
	else
		this->_serverList[this->_serverList.size() - 1].getServerConfig().setRoot(val);
	// (void)pos_end;
	return (end + 1);
}

size_t ParsingConfigFile::checkAllowMethods(size_t pos_start, size_t pos_end, bool controlFlag)
{
	(void)pos_end;
	size_t end;
	bool g = false, p = false, d = false;

	std::vector<std::string> words;

	std::string methods = this->_data.substr(pos_start, this->_data.find('\n', pos_start) - pos_start);
	if ((end = methods.find(';') ) == std::string::npos)
		throw MyException("Error Sintexsis Allow_method");
	methods = methods.substr(0, end);
	std::istringstream iss(methods);
	
	std::string word;
	while (iss >> word)
		words.push_back(word);

	if (words.empty())
		throw MyException("Error Sintexsis Allow_method");

	for (size_t i = 0; i < words.size(); i++)
	{
		if (words[i] != "GET" && words[i] != "POST" && words[i] != "DELETE")
            throw MyException("Error Sintexsis Allow_method");
		if (words[i] == "GET")
			g = true;
		if (words[i] == "POST")
			p = true;
		if (words[i] == "DELETE")
			d = true;
	}
	
	if (!controlFlag)
	{
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getAllow_methods().setGet(g);
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getAllow_methods().setPost(p);
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getAllow_methods().setDelete(d);
	}
	else
	{
		this->_serverList[this->_serverList.size() - 1].getLocations()[this->_serverList[this->_serverList.size() - 1].\
		getLocations().size() - 1].getAllow_methods().setGet(g);
		this->_serverList[this->_serverList.size() - 1].getLocations()[this->_serverList[this->_serverList.size() - 1].\
		getLocations().size() - 1].getAllow_methods().setPost(p);
		this->_serverList[this->_serverList.size() - 1].getLocations()[this->_serverList[this->_serverList.size() - 1].\
		getLocations().size() - 1].getAllow_methods().setDelete(d);
	}
	return size_t(end + 1);
}

size_t ParsingConfigFile::checkUploadDir(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis Upload_Dir");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis Upload_Dir");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, check;
	str >> val;
	str >> check;
	if (check.size())
		throw MyException("Error Sintexsis Upload_Dir arguments");
	DIR *dir = opendir(val.c_str());
	if (dir)
		throw MyException("Error Sintexsis Upload_Dir not direktory");
	closedir(dir);
	if (controlFlag)
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].setUpload_dir(val);
	else
		this->_serverList[this->_serverList.size() - 1].getServerConfig().setUpload_dir(val);
	return (end + 1);
}


size_t ParsingConfigFile::checkErrorPage(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis ErrorPage");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis ErrorPage");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, page;
	std::vector<size_t> statCod;

	for (; !str.eof() ;)
	{
		str >> val;
		if (!val.size())
			continue;
		try
		{
			statCod.push_back(statusCodes(val));
		}
		catch(...)
		{
			if(page.size() || !statCod.size())
				throw MyException("Error Sintexsis ErrorPage");
			if (access(val.c_str(), F_OK))
				throw MyException("Error Sintexsis ErrorPage no file");
			page = val;
		}
	}
	if (!statCod.size() || !page.size())
		throw MyException("Error Sintexsis ErrorPage");
	std::map<int, std::string> *_map;
	if (controlFlag)
	{
		_map = &(this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size()].getError_page());
	}
	else{
		_map = &(this->_serverList[this->_serverList.size() - 1].getServerConfig().getError_page());
	}
	for (size_t i = 0; i < statCod.size(); ++i)
	{
		_map->insert(std::pair<size_t, std::string>(statCod[i],val));
	}
	return (end + 1);
}

size_t ParsingConfigFile::statusCodes(std::string number)
{
	size_t  size = number.size();
	int num;
	if (!size)
		throw MyException(" ");
	try
	{
		num = std::stoi(number, &size);
	}
	catch(...)
	{
		throw MyException(" ");
	}
	if (size != number.size() || num < 100 || num > 599)
		throw MyException(" ");
	return (static_cast<size_t>(num));
}

size_t ParsingConfigFile::checkReturn(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis Return");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis Return");

	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, page;
	size_t		statCod = 0, ival = 0;
	for (; !str.eof() ;)
	{
		str >> val;
		if (!val.size())
			continue;
		try
		{
			ival = statusCodes(val);
		}
		catch(...)
		{
			if(page.size() || !statCod)
				throw MyException("Error Sintexsis Return");
			page = val;
			continue;
		}
		if (ival && statCod)
			throw MyException("Error Sintexsis Return");
		if (ival && !statCod)
			statCod = ival;
		ival = 0;
	}
	if (controlFlag)
	{
		//karoxa size -1
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size()].getReturn().setStatusTypes(statCod);
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size()].getReturn().setPath(val);
	}
	else{
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getReturn().setStatusTypes(statCod);
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getReturn().setPath(val);
	}
	return (end + 1);
}

size_t ParsingConfigFile::checkCgi(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis Upload_Dir");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis Upload_Dir");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, check;
	str >> val;
	str >> check;
	if (check.size())
		throw MyException("Error Sintexsis Upload_Dir arguments");
	bool *res;
	if (controlFlag)
		res = &(this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].getCgi());
	else
		res = &(this->_serverList[this->_serverList.size() - 1].getServerConfig().getCgi());
	std::transform(val.begin(), val.end(), val.begin(), ::tolower);
	if (val == std::string("on"))
		*res = (true);
	else if(val == std::string("off"))
		*res = (false);
	else
		throw MyException("Error Sintexsis Upload_Dir arguments");
	return (end + 1);
}

size_t ParsingConfigFile::checkIndex(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	for (; i < pos_end; i++)
	{
		if (this->_data[i] == ';')
		{
			end = i;
			break;
		}
		if (this->_data[i] == '\n')
			throw MyException("Error Sintexsis Index");
	}
	if (i >= pos_end)
		throw MyException("Error Sintexsis Index");
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val;
	std::vector<std::string> *res;
	if (controlFlag)
		res = &(this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].getIndex());
	else
		res = &(this->_serverList[this->_serverList.size() - 1].getServerConfig().getIndex());
	res->clear();
	for (; !str.eof() ;)
	{
		str >> val;
		if (!val.size())
			continue;
		res->push_back(val);
	}
	if (!res->size())
		throw MyException("Error Sintexsis Index");
	return (end + 1);
}

size_t ParsingConfigFile::checkAutoindex(size_t pos_start, size_t pos_end, bool controlFlag)
{
	(void)pos_end;
	size_t end;
	std::string autoindex = this->_data.substr(pos_start, this->_data.find('\n', pos_start) - pos_start);
	if ((end = autoindex.find(';') ) == std::string::npos)
		throw MyException("Error Sintexsis Autoindex");
	autoindex = autoindex.substr(0, end);
	std::string check;
	std::istringstream iss(autoindex);
	iss >> autoindex;
	iss >> check;
	if (check.size())
		throw MyException("Error Sintexsis Autoindex arguments");
	
	if (autoindex != "off" && autoindex != "on")
		throw MyException("Error Sintexsis Autoindex");//eli qnnarkel sosi het
	if (!controlFlag)
		this->_serverList[this->_serverList.size() - 1].getServerConfig().setAutoindex(autoindex == "off" ? false : true);
	else
		(this->_serverList[this->_serverList.size() - 1].getLocations())[this->_serverList[this->_serverList.size() - 1].\
		getLocations().size() - 1].setAutoindex(autoindex == "off" ? false : true);
	return size_t(end + 1);
}

size_t ParsingConfigFile::checkGigabyte(std::string &BodySize)
{
	size_t numb = std::stoul(BodySize);
	std::string result = std::to_string(numb);
	if (result.size() > 11 || numb > 17179869183)
		throw MyException("Error Sintexsis ClientBodySize");
	return (numb);
}

size_t ParsingConfigFile::checkMegabyte(std::string &BodySize)
{
	size_t numb = std::stoul(BodySize);
	std::string result = std::to_string(numb);
	if (result.size() > 14 || numb > 17592186044415)
		throw MyException("Error Sintexsis ClientBodySize");
	return (numb);
}

size_t ParsingConfigFile::checkKilobyte(std::string &BodySize)
{
	size_t numb = std::stoul(BodySize);
	std::string result = std::to_string(numb);
	if (result.size() > 17 || numb > 18014398509481983)
		throw MyException("Error Sintexsis ClientBodySize");
	return (numb);
}

size_t ParsingConfigFile::checkByte(std::string &BodySize)
{
	size_t nonZeroPos = BodySize.find_first_not_of('0');

    if (nonZeroPos != std::string::npos)
        BodySize.erase(0, nonZeroPos);
     else 
        BodySize = "0";

	size_t numb = static_cast<size_t>(std::strtoull(BodySize.c_str(), nullptr, 10));
	std::string check = std::to_string(numb);
	if (check != BodySize)
		throw MyException("Error Sintexsis ClientBodySize");
	return (numb);
}

size_t ParsingConfigFile::findSimbol(std::string &BodySize)
{
	size_t pos = BodySize.find_first_not_of("0123456789");
	size_t result;
	if (pos == std::string::npos)
		result = checkByte(BodySize);
	else if (BodySize[pos] == 'K' || BodySize[pos] == 'k' && BodySize[pos + 1] == '\0')
		result = checkKilobyte(BodySize);
	else if (BodySize[pos] == 'M' || BodySize[pos] == 'm' && BodySize[pos + 1] == '\0')
		result = checkMegabyte(BodySize);
	else if (BodySize[pos] == 'G' || BodySize[pos] == 'g' && BodySize[pos + 1] == '\0')
		result = checkGigabyte(BodySize);
	else
		throw MyException("Error Sintexsis ClientBodySize");
	return (result);
}

size_t ParsingConfigFile::checkClientMaxBodySize(size_t pos_start, size_t pos_end, bool controlFlag)
{
	size_t end;
	size_t result;
	std::string BodySize = this->_data.substr(pos_start, this->_data.find('\n', pos_start) - pos_start);
	if ((end = BodySize.find(';') ) == std::string::npos)
		throw MyException("Error Sintexsis ClientBodySize");
	BodySize = BodySize.substr(0, end);
	std::string check;
	std::istringstream iss(BodySize);
	iss >> BodySize;
	iss >> check;
	if (check.size())
		throw MyException("Error Sintexsis Autoindex arguments");
	result = findSimbol(BodySize);

	if (controlFlag)
	{
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].setClient_max_body_size(result);//karoxa size-1 kam che
	}
	else
		this->_serverList[this->_serverList.size() - 1].getServerConfig().setClient_max_body_size(result);

	return size_t(end + 1);
}

size_t ParsingConfigFile::runSpaceTab(size_t pos_start, size_t pos_end)
{
	size_t	i = pos_start;
	for (; i < pos_end; ++i)
	{
		if (this->_data[i] != '\t' && this->_data[i] != ' ')
		{
			return (i);
		}
	}
	return (SIZE_MAX);
}

/*
	server {  lisen             127.0.0.1:8080   ;  }
         		   				^pos_start    ^pos_end

*/

bool ParsingConfigFile::checCorectHostAndPort(size_t pos_start, size_t pos_end, bool controlFlag)
{
	(void)controlFlag;
	size_t	pos_period = this->_data.find(":", pos_start, pos_end - pos_start);
	if (pos_period == SIZE_MAX)
		throw MyException("Error Sintexsis Listen -> :");
	if (!chekAndSaveHost(std::string(this->_data.begin() + pos_start, this->_data.begin() + pos_period)))
		throw MyException("Error Sintexsis Listen Host");
	if (!chekAndSavePort(std::string(this->_data.begin() + pos_period + 1, this->_data.begin() + pos_end)))
		throw MyException("Error Sintexsis Listen Host");
	return (true);
}

bool u(char a)
{
	return (a == '.');
}

bool ParsingConfigFile::chekAndSaveHost(std::string host)
{
	size_t count = std::count_if(host.begin(), host.end(), u);
	std::stringstream str(host);
	std::string val;
	if (count == 3)
	{
		for (size_t i = 0; i < 4; i++)
		{
			val.clear();
			std::getline(str, val, '.');
			if (!chekHostNumber(val))
				throw MyException("Error Sintexsis Listen Host");
		}
	}
	else
		throw MyException("Error Sintexsis Listen Host -> . <-");
	this->_serverList[this->_serverList.size() - 1].setHost(host);
	return (true);
}

bool ParsingConfigFile::chekAndSavePort(std::string port)
{
	size_t  size = port.size();
	int number;
	if (!size)
		return (false);
	try
	{
		number = std::stoi(port, &size);
	}
	catch(...)
	{
		throw MyException("Error Sintexsis Listen Host");
	}
	if (size != port.size() || number < 0)
		throw MyException("Error Sintexsis Listen Host");
	this->_serverList[this->_serverList.size()].setPort(number);
	return (true);
}

bool ParsingConfigFile::chekHostNumber(std::string number)
{
	size_t  size = number.size();
	int num;
	if (!size)
		return (false);
	try
	{
		num = std::stoi(number, &size);
	}
	catch(...)
	{
		throw MyException("Error Sintexsis Listen Host");
	}
	if (size != number.size() || num < 0 || num > 255)
		throw MyException("Error Sintexsis Listen Host");
	return (true);
}


ParsingConfigFile::MyException::MyException(const std::string &error) : _error(error) {}

ParsingConfigFile::MyException::~MyException() throw() {}

const char * ParsingConfigFile::MyException::what() const throw() { return ((this->_error.data())); }