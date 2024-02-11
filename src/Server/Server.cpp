#include "Server.hpp"

Server::Server():_host("127.0.0.1"),_host_flag(false),_port_flag(false)
 {
	this->_port.push_back(8080);
 }

Server::Server(const Server& other):
	_serverConfig(other._serverConfig), _serverName(other._serverName),
	_host(other._host),_host_flag(other._host_flag), _port(other._port), _port_flag(other._port_flag),_locations(other._locations) {}

Server& Server::operator= (const Server& other)
{
	if (this != &other)
	{
		this->_serverConfig = other._serverConfig;
		this-> _serverName = other._serverName;
		this->_host = other._host;
		this->_host_flag = other._host_flag;
		this->_port = other._port;
		this->_port_flag = other._port_flag;
		this->_locations = other._locations;
	}
	return (*this);
}

Server::~Server() {}

Config& Server::getServerConfig()
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

bool Server::getHostFlag()
{
	return (this->_host_flag);
}

Server &Server::setHostFlag(const bool &host)
{
	this->_host_flag = host;
	return (*this);
}

std::vector<size_t>& Server::getPort()
{
	return (this->_port);
}

Server& Server::setPort(const std::vector<size_t> port)
{
	this->_port = port;
	return (*this);
}

bool Server::getPortFlag()
{
	return (this->_port_flag);
}

Server &Server::setPortFlag(const bool port)
{
	this->_port_flag = port;
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

std::vector<size_t> &Server::getMainServer()
{
    return (this->_mainServer);
}

Server &Server::setMainServer(const std::vector<size_t> &fd)
{
	this->_mainServer = fd;
	return (*this);
}

std::vector<size_t> &Server::getClientSocket()
{
    return (this->_clientSocket);
}

Server &Server::setClientSocket(const std::vector<size_t> &fd)
{
	this->_clientSocket = fd;
	return (*this);
}
