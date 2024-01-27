#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <iostream>
#include <string>
#include "ActiveMetods.hpp"
#include "StatusCollection.hpp"
#include "CgiHandler.hpp"

class Config
{
private:
	std::string _root;
	ActiveMetods _allow_methods;
	std::string _upload_dir;
	StatusCollection _error_page;
	StatusCollection _return;
	CgiHandler _cgi;
	std::string _index;
	std::string _location_name;
	bool _autoindex;
	unsigned long _client_max_body_size;
public:
	Config();
	Config(const Config& other);
	Config& operator= (const Config& other);
	~Config();

	std::string getRoot() const;
	Config& setRoot(const std::string& root);

	const ActiveMetods& getAllow_methods() const;
	Config& setAllow_methods(const ActiveMetods& allow_methods);

	std::string getUpload_dir() const;
	Config& setUpload_dir(const std::string& upload_dir);

	const StatusCollection& getError_page() const;
	Config& setError_page(const StatusCollection& error_page);

	const StatusCollection& getReturn() const;
	Config& setReturn(const StatusCollection& _return);

	const CgiHandler&  getCgi() const;
	Config& setCgi(const CgiHandler& cgi);

	std::string getIndex() const;
	Config& setIndex(const std::string& index);

	std::string getLocation_name() const;
	Config& setLocation_name(const std::string& location_name);

	bool getAutoindex() const;
	Config& setAutoindex(const bool autoindex);

	unsigned long getClient_max_body_size() const;
	Config& setClient_max_body_size(const unsigned long client_max_body_size);
};


#endif