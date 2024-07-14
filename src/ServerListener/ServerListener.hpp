#ifndef _LISEN_STRUC_HPP_
#define _LISEN_STRUC_HPP_

#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "Logger.hpp"

class ServerListener
{
private:
	int					_server;
	struct sockaddr_in	_serverAddress;
public:
	ServerListener(size_t port, std::string host);
	ServerListener(const ServerListener& other);
	ServerListener & operator=(const ServerListener& other);
	~ServerListener();

	bool	bind();
	bool	listen();
	void	close();
	int		getServerFD();
	struct sockaddr_in & getServerAddress();
};

#endif