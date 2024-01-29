#include "Server.hpp"

Server::Server():_port(0) {}

Server::Server(const Server& other):
	_serverConfig(other._serverConfig), _serverName(other._serverName),
	_host(other._host),_port(other._port),_locations(other._locations) {}

Server& Server::operator= (const Server& other)
{
	if (this != &other)
	{
		this->_serverConfig = other._serverConfig;
		this-> _serverName = other._serverName;
		this->_host = other._host;
		this->_port = other._port;
		this->_locations = other._locations;
	}
	return (*this);
}

Server::~Server() {}

const Config& Server::getServerConfig() const
{
	return (this->_serverConfig);
}

Server& Server::setServerConfig(const Config& server_config)
{
	this->_serverConfig = server_config;
	return (*this);
}

std::string Server::getServerName() const
{
	return (this->_serverName);
}

Server& Server::setServerName(const std::string& server_name)
{
	this->_serverName = server_name;
	return (*this);
}

std::string Server::getHost() const
{
	return (this->_host);
}

Server& Server::setHost(const std::string& host)
{
	this->_host = host;
	return (*this);
}

unsigned int Server::getPort() const
{
	return (this->_port);
}

Server& Server::setPort(const unsigned int port)
{
	this->_port = port;
	return (*this);
}

std::vector<Config>& Server::getLocations()
{
	return (this->_locations);
}

Server& Server::setLocations(const std::vector<Config>& locations)
{
	this->_locations = locations;
	return (*this);
}