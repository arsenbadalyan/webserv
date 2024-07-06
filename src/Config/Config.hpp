#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <iostream>
#include <string>
#include <map>
#include "ActiveMetods.hpp"
#include "StatusCollection.hpp"
#include "CgiHandler.hpp"

class Config
{
private:
	std::string					_root;
	bool						_root_flag;
	ActiveMetods				_allow_methods;
	std::string					_upload_dir;
	std::map<int, std::string>	_error_page;
	StatusCollection			_return;
	// bool						_cgi;
	std::vector<std::string>	_index;
	std::string					_location_name;
	bool						_autoindex;
	bool						_cmbs_flaf;
	unsigned long				_client_max_body_size;
public:
	Config();
	Config(const Config& other);
	Config& operator= (const Config& other);
	~Config();

	std::string getRoot() const;
	Config& setRoot(const std::string& root);

	bool	getRootFlag();
	Config& setRootFlag(const bool root_flag);

	ActiveMetods& getAllow_methods();
	Config& setAllow_methods(const ActiveMetods& allow_methods);

	std::string getUpload_dir() const;
	Config& setUpload_dir(const std::string& upload_dir);

	std::map<int, std::string> & getError_page() ;
	Config& setError_page(const std::map<int, std::string>& error_page);

	StatusCollection& getReturn();
	Config& setReturn(const StatusCollection& _return);

	bool&  getCgi();
	Config& setCgi(const bool& cgi);

	std::vector<std::string> & getIndex();
	Config& setIndex(const std::vector<std::string>& index);

	std::string getLocation_name() const;
	Config& setLocation_name(const std::string& location_name);

	bool getAutoindex() const;
	Config& setAutoindex(const bool autoindex);

	bool getcmbsFlag() const;
	Config& setcmbsFlag(const bool cmbs_flaf);

	unsigned long getClient_max_body_size() const;
	Config& setClient_max_body_size(const unsigned long client_max_body_size);
};


#endif