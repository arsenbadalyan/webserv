#include "Config.hpp"

Config::Config():
	_root("/var/www"),
	_root_flag(false),
	_autoindex(true),_client_max_body_size(1024*1024), _cmbs_flaf(false)
{
	this->_index.push_back("index.html");
	
}


Config::Config(const Config& other):
	_root(other._root),
	_root_flag(other._root_flag),
	_allow_methods(other._allow_methods),
	_upload_dir(other._upload_dir),
	_error_page(other._error_page),
	_return(other._return), _cgi(other._cgi) ,
	_index(other._index),_location_name(other._location_name),
	_autoindex(other._autoindex),
	_cmbs_flaf(other._cmbs_flaf),
	_client_max_body_size(other._client_max_body_size) {}

Config& Config::operator= (const Config& other)
{
	if (this != &other)
	{
		this->_root = other._root;
		this->_root_flag = other._root_flag;
		this->_allow_methods = other._allow_methods;
		this->_upload_dir = other._upload_dir;
		this->_error_page = other._error_page;
		this->_return = other._return;
		this->_cgi = other._cgi;
		this->_index = other._index;
		this->_location_name = other._location_name;
		this->_autoindex = other._autoindex;
		this->_cmbs_flaf = other._cmbs_flaf;
		this->_client_max_body_size = other._client_max_body_size;
	}
	return (*this);
}

Config::~Config() {}

std::string Config::getRoot() const
{
	return (this->_root);
}

Config& Config::setRoot(const std::string& root)
{
	this->_root = root;
	return (*this);
}

bool Config::getRootFlag()
{
	return (this->_root_flag);
}

Config &Config::setRootFlag(const bool root_flag)
{
	this->_root_flag = root_flag;
	return (*this);
}

ActiveMetods& Config::getAllow_methods()
{
	return (this->_allow_methods);
}
	
Config& Config::setAllow_methods(const ActiveMetods& allow_methods)
{
	this->_allow_methods = allow_methods;
	return (*this);
}

std::string Config::getUpload_dir() const
{
	return (this->_upload_dir);
}

Config& Config::setUpload_dir(const std::string& upload_dir)
{
	this->_upload_dir = upload_dir;
	return (*this);
}

std::map<int, std::string>& Config::getError_page()
{
	return (this->_error_page);
}

Config& Config::setError_page(const std::map<int, std::string>& error_page)
{
	this->_error_page = error_page;
	return (*this);
}

StatusCollection& Config::getReturn()
{
	return (this->_return);
}

Config& Config::setReturn(const StatusCollection& _return)
{
	this->_return = _return;
	return (*this);
}

bool&  Config::getCgi()
{
	return (this->_cgi);
}

Config& Config::setCgi(const bool& cgi)
{
	this->_cgi = cgi;
	return (*this);
}

std::vector<std::string>& Config::getIndex()
{
	return (this->_index);
}

Config& Config::setIndex(const std::vector<std::string>& index)
{
	this->_index = index;
	return (*this);
}

std::string Config::getLocation_name() const
{
	return (this->_location_name);
}

Config& Config::setLocation_name(const std::string& location_name)
{
	this->_location_name = location_name;
	return (*this);
}

bool Config::getAutoindex() const
{
	return (this->_autoindex);
}

Config& Config::setAutoindex(const bool autoindex)
{
	this->_autoindex = autoindex;
	return (*this);
}

bool Config::getcmbsFlag() const
{
	return (this->_cmbs_flaf);
}

Config& Config::setcmbsFlag(const bool cmbs_flaf)
{
	this->_cmbs_flaf = cmbs_flaf;
	return (*this);
}

unsigned long Config::getClient_max_body_size() const
{
	return (_client_max_body_size);
}

Config& Config::setClient_max_body_size(const unsigned long client_max_body_size)
{
	this->_client_max_body_size = client_max_body_size;
	return (*this);
}