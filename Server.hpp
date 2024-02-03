#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include "Config.hpp"
#include <vector>

class Server 
{
private:
	Config _serverConfig;
	std::vector<std::string> _serverName;
	std::string	_host;
	unsigned int _port;
	std::vector<Config> _locations;
public:
	Server();
	Server(const Server& other);
	Server& operator= (const Server& other);
	~Server();

	Config& getServerConfig();
	Server& setServerConfig(const Config& server_config);

	std::vector<std::string>& getServerName();
	Server& setServerName(const std::vector<std::string>& server_name);

	std::string getHost() const;
	Server& setHost(const std::string& host);

	unsigned int getPort() const;
	Server& setPort(const unsigned int port);

	std::vector<Config>& getLocations();
	Server& setLocations(const std::vector<Config>& locations);

};

#endif 