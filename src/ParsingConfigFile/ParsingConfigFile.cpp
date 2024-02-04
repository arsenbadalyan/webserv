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

std::vector<Server> ParsingConfigFile::startPars()
{
	std::cout << "error -> 1" << std::endl;
	this->_data = this->_file.Read();
	std::cout << "error -> 2" << std::endl;
	if (!this->_data.size())
		throw MyException("Emty File ERROR");
	std::cout << "error -> 3" << std::endl;
	if (!this->CheckSintex())
		throw MyException("Sintexs ERROR");
	std::cout << "error -> 4" << std::endl;
	if (!this->CheckCorecktServer())
		throw MyException("Corect Error");
	std::cout << "error -> 5" << std::endl;
	return (this->_serverList);
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
	std::cout << "error -> 1.1" << std::endl;
	for (size_t pos_start = 0, pos_end = 0, pos = 0; \
		pos_start < this->_data.size(); pos_start = pos_end + 1)
	{
		std::cout << "error -> 1.2" << std::endl;
		pos_end = this->_data.find('{', pos_start);
		if (SIZE_MAX == pos_end && pos_start == 0)
			throw MyException("Suntexs Error");
		std::cout << "error -> 1.3" << std::endl;
		if (SIZE_MAX == pos_end)
		{
			for (size_t i = pos_start; i < this->_data.size() ; ++i)
			{
				if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
					throw MyException("Suntexs Error");
			}
			return (true);
		}
		std::cout << "error -> 1.4" << std::endl;
		std::transform(this->_data.begin(), this->_data.begin() + (pos_end - pos_start), \
			this->_data.begin(), ::tolower);
		pos = this->_data.find(server.c_str(), pos_start, pos_end - pos_start);
		if (SIZE_MAX == pos_end)
			throw MyException("Suntexs Error don exist server");
		std::cout << "error -> 1.5" << std::endl;
		for (size_t i = pos_start; i < pos_end; ++i)
		{
			if (i < pos && i > pos + server.size() && \
				this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
				{
					// std::cout << "i = " << i  << "  " << (int)this->_data[i] << std::endl;
					throw MyException("Suntexs Error");
				}
		}
		std::cout << "error -> 1.6" << std::endl;
		pos_start = pos_end + 1;
		pos_end = get_reng(pos_start);
		if (pos_end == SIZE_MAX)
			throw MyException("Suntexs Error");
		std::cout << "error -> 1.6" << std::endl;
		if (!CheckCoreckt(pos_start, pos_end, false))
			throw MyException("Suntexs Error");
		std::cout << "error -> 1.7" << std::endl;
		if (pos_end == this->_data.size())
			return (true);
		std::cout << "error -> 1.8" << std::endl;
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
	std::cout << "error -> 2.1" << std::endl;
	if (!controlFlag)
		this->_serverList.push_back(Server());
	// else
	// 	this->_serverList[this->_serverList.size() -1].getLocations().push_back(Config());
	std::string cpy;
	std::cout << "error -> 2.2" << std::endl;
	for (size_t i = pos_start, pos = 0; i < pos_end;)
	{
		std::cout << "error -> 2.3" << std::endl;
		if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ' )
		{
			pos = i;
			std::cout << "error -> 2.4" << std::endl;
			for (; this->_data[i] != '\t' && this->_data[i] != ' ' && this->_data[i] != '}'; i++);
			if (this->_data[i] == '}')
				throw MyException("Emty Lokacion");
			std::cout << "error -> 2.5" << std::endl;
			cpy =  this->_data.substr(pos, i - pos);
			std::transform(cpy.begin(), cpy.end(), cpy.begin(), ::tolower);
			std::cout << "error -> 2.6" << std::endl;
			write(1,"error read -> 2.6\n", 19);
			if (std::string("location") == cpy)
			{
				std::cout << "error -> 2.7" << std::endl;
				if (controlFlag)
					throw MyException("Error double location");
				this->_serverList[this->_serverList.size() -1].getLocations().push_back(Config());
				i = CheckCorecktLocation(i, pos_end);
				std::cout << "error -> 2.8" << std::endl;
			}
			else
				i = CheckCorecktConfig(cpy, i, pos_end, controlFlag);
			std::cout << "error -> 2.8" << std::endl;
			if (SIZE_MAX == i)
				throw MyException("Suntexs Error");
			std::cout << "error -> 2.9" << std::endl;
		}
		else
			++i;
		cpy.clear();
		std::cout << "error -> 2.10" << std::endl;
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
			if (!CheckCoreckt(start, end, true))
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
	std::cout << "conf = " << config << "  start = " << pos_start << "  end == " << pos_end << std::endl;
	std::cout << "error -> 3.1" << std::endl;
	size_t res = SIZE_MAX;
	// std::cout << "config = " << config << std::endl;
	if (std::string("listen") == config)
	{
		std::cout << "error -> 3.2" << std::endl;
		if (controlFlag)
			throw MyException("Error listen for location");
		res = checkListen(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	} 
	else if (std::string("server_name") == config)
	{
		std::cout << "error -> 3.3" << std::endl;
		if (controlFlag)
			throw MyException("Error server_name for location");
		res = checkServerName(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	} 
	else if (std::string("root") == config)
	{
		std::cout << "error -> 3.4" << std::endl;
		res = checkRoot(pos_start, pos_end, controlFlag);
		std::cout << "error -> 3.4.2" << std::endl;
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("allow_method") == config)
	{
		std::cout << "error -> 3.5" << std::endl;
		res = checkAllowMethods(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("upload_dir") == config)
	{
		std::cout << "error -> 3.6" << std::endl;
		res = checkUploadDir(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("error_page") == config)
	{
		std::cout << "error -> 3.7" << std::endl;
		res = checkErrorPage(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("return") == config)
	{
		std::cout << "error -> 3.8" << std::endl;
		res = checkReturn(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("cgi") == config)
	{
		std::cout << "error -> 3.9" << std::endl;
		res = checkCgi(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("index") == config)
	{
		std::cout << "error -> 3.10" << std::endl;
		res = checkIndex(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("autoindex") == config)
	{
		std::cout << "error -> 3.11" << std::endl;
		res = checkAutoindex(pos_start, pos_end, controlFlag);
		std::cout << "res == " << res << std::endl;
		if (SIZE_MAX != res)
			return (res);
	}
	else if (std::string("client_max_body_size") == config)
	{
		std::cout << "error -> 3.12" << std::endl;
		res = checkClientMaxBodySize(pos_start, pos_end, controlFlag);
		if (SIZE_MAX != res)
			return (res);
	}
	std::cout << "error -> 3.15" << std::endl;
	std::cout << "conf = " << "  start = " << pos_start << "  end == " << pos_end << std::endl;
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
	// std::cout << "start = " << start << "  End = " << end << std::endl;
	for (; start < end && (this->_data[start] == '\t' || this->_data[start] == ' '); ++start);
	if (start >= end)
		throw MyException("Error Emty Listen");
	// std::cout << "start = " << start << "  End = " << end << std::endl;
	// std::cout << "--------------->>> |" << this->_data[start] << "|\n";
	// std::cout <<"|"<< std::string(this->_data.begin() + start,this->_data.end()) << std::endl;
	for (end = start; end < pos_end && this->_data[end] != '\t' && this->_data[end] != ' '; ++end);
	// std::cout <<"|"<<  std::string(this->_data.begin() + end,this->_data.end()) << std::endl;
	// std::cout << std::string(this->_data.begin() + pos_end,this->_data.end()) << std::endl;
	for (i = end; i < pos_end; ++i)
		if (this->_data[i] != '\t' && this->_data[i] != ' ')
			throw MyException("Error Sintexts Listen1");
	if (!checCorectHostAndPort(start, end, controlFlag))
		throw MyException("Error Sintexts Listen2");
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
	std::cout << "error -> 5.1" << std::endl;
	size_t end = 0;
	// size_t start = pos_start;
	size_t i = pos_start;
	std::cout << "error -> 5.2" << std::endl;
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
	std::cout << "error -> 5.3" << std::endl;
	if (i >= pos_end)
		throw MyException("Error Sintexsis root");
	std::cout << "error -> 5.4" << std::endl;
	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, check;
	std::cout << "error -> 5.5" << std::endl;
	str >> val;
	str >> check;
	std::cout << "error -> 5.6" << std::endl;
	if (check.size())
		throw MyException("Error Sintexsis root arguments");
	std::cout << "error -> 5.7" << std::endl;
	DIR *dir = opendir(val.c_str());
	if (!dir)
		throw MyException("Error Sintexsis root not direktory");
	std::cout << "error -> 5.8" << std::endl;
	write(1,"error read -> 5.8\n", 19);
	std::cout << "dir == -> " << dir << std::endl;
	closedir(dir);
	write(1,"error read -> 5.9\n", 19);
	std::cout << "error -> 5.9" << std::endl;
	if (controlFlag)
	{
		std::cout << "error -> 5.10" << std::endl;
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].setRoot(val);
	}
	else
	{
		std::cout << "error -> 5.11" << std::endl;
		this->_serverList[this->_serverList.size() - 1].getServerConfig().setRoot(val);
	}
	std::cout << "error -> 5.15" << std::endl;
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
	std::cout << std::string(this->_data.begin() + end + pos_start, this->_data.end()) << std::endl;
	return size_t(end + 1 + pos_start);
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
	std::cout << "error -> 5." << std::endl;
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
	std::cout << "error -> 5.4" << std::endl;
	if (i >= pos_end)
		throw MyException("Error Sintexsis Return");

	std::stringstream str(std::string(this->_data.begin() + pos_start, this->_data.begin() + end));
	std::string val, page;
	size_t		statCod = 0, ival = 0;
	std::cout << "error -> 5.7" << std::endl;
	for (; !str.eof() ;)
	{
		std::cout << "error -> 5.8" << std::endl;
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
		std::cout << "error -> 5.12" << std::endl;
	}
	if (controlFlag)
	{
		std::cout << "error -> 5.15" << std::endl;
		// std::cout << "Size = -->>  " << this->_serverList.size() << std::endl;
		// std::cout << "error -> 5.15" << std::endl;
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].getReturn().setStatusTypes(statCod);
		this->_serverList[this->_serverList.size() - 1].getLocations()\
		[this->_serverList[this->_serverList.size() - 1].getLocations().size() - 1].getReturn().setPath(val);
	}
	else{
		std::cout << "error -> 5.17" << std::endl;
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getReturn().setStatusTypes(statCod);
		this->_serverList[this->_serverList.size() - 1].getServerConfig().getReturn().setPath(val);
	}
	std::cout << "error -> 5.20" << std::endl;
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
	std::cout << std::string(this->_data.begin() + end + pos_start, this->_data.end()) << std::endl;
	return size_t(end + 1 + pos_start);
}

size_t ParsingConfigFile::checkClientMaxBodySize(size_t pos_start, size_t pos_end, bool controlFlag)
{
	(void)pos_end;
	(void)pos_start;
	(void)controlFlag;
	return size_t(0);
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
	// std::cout << "start = " << pos_start << "  End = " << pos_end << std::endl;
	// std::cout << std::string(this->_data.begin() + pos_start,this->_data.end()) << std::endl;
	std::string::iterator  it = std::find(this->_data.begin() + pos_start, this->_data.begin() + pos_end, ':');
	if (it == this->_data.end())
		throw MyException("Error Sintexsis Listen -> :");
	size_t	pos_period = it - this->_data.begin();
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

/////////////////

ParsingConfigFile::MyException::MyException(const std::string &error) : _error(error) {}

ParsingConfigFile::MyException::~MyException() throw() {}

const char * ParsingConfigFile::MyException::what() const throw() { return ((this->_error.data())); }