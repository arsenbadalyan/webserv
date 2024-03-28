#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include "Config.hpp"
#include "LisenStruc.hpp"
#include <vector>
#include "Util.hpp"

class Server 
{
public:

	struct ReadSoket
	{
		int		_fd;
		size_t	_flag;
		ReadSoket(int fd, size_t flag) :\
		 _fd(fd), _flag(flag){}
	};

private:

	Config					_serverConfig;
	std::string				_serverName;
	std::string				_host;
	bool					_host_flag;
	std::vector<size_t> 	_port;
	bool					_port_flag;
	std::vector<Config> 	_locations;
	std::vector<LisenStruc>	_mainServer;
	std::vector<int>		_writSocket;
	std::vector<ReadSoket>	_readSocket;
public:
	Server();
	Server(const Server& other);
	Server& operator= (const Server& other);
	~Server();

	Config*	findLocations(const std::string & name);

	Config& getServerConfig();
	Server& setServerConfig(const Config& server_config);

	std::string getServerName() const;
	Server& setServerName(const std::string& server_name);

	std::string getHost() const;
	Server& setHost(const std::string& host);

	bool getHostFlag();
	Server& setHostFlag(const bool& host);

	std::vector<size_t>& getPort();
	Server& setPort(const std::vector<size_t> port);

	bool getPortFlag();
	Server& setPortFlag(const bool port);

	std::vector<Config>& getLocations();
	Server& setLocations(const std::vector<Config>& locations);

	std::vector<LisenStruc>& getMainServer();
	Server& setMainServer(const std::vector<LisenStruc>& fd);

	std::vector<int>& getWritSocket();
	Server& setWritSocket(const std::vector<int>& fd);

	std::vector<ReadSoket>& getReadSocket();
	Server& setReadSocket(const std::vector<ReadSoket>& fd);

};

#endif 