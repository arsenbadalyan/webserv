#include "Server.hpp"

Server::Server():_port(0) {}

Server::Server(const Server& other):
	_server_config(other._server_config),_server_name(other._server_name),
	_host(other._host),_port(other._port),_locations(other._locations) {}

Server& Server::operator= (const Server& other)
{
	if (this != &other)
	{
		this->_server_config = other._server_config;
		this-> _server_name = other._server_name;
		this->_host = other._host;
		this->_port = other._port;
		this->_locations = other._locations;
	}
	return (*this);
}

Server::~Server() {}

const Config& Server::getServer() const
{
	return (this->_server_config);
}

Server& Server::setServer(const Config& server_config)
{
	this->_server_config = server_config;
	return (*this);
}

std::string Server::getServer_name() const
{
	return (this->_server_name);
}

Server& Server::setServer_name(const std::string& server_name)
{
	this->_server_name = server_name;
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