#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include "Config.hpp"
#include <vector>

class Server 
{
private:
	Config _server_config;
	std::string	 _server_name;
	std::string	_host;
	unsigned int _port;
	std::vector<Config> _locations;
public:
	Server();
	Server(const Server& other);
	Server& operator= (const Server& other);
	~Server();

	const Config& getServer() const;
	Server& setServer(const Config& server_config);

	std::string getServer_name() const;
	Server& setServer_name(const std::string& server_name);

	std::string getHost() const;
	Server& setHost(const std::string& host);

	unsigned int getPort() const;
	Server& setPort(const unsigned int port);

	std::vector<Config>& getLocations();
	Server& setLocations(const std::vector<Config>& locations);

};

#endif 