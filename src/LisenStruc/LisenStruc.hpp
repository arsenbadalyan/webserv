#ifndef _LISEN_STRUC_HPP_
#define _LISEN_STRUC_HPP_

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>



class LisenStruc
{
private:
	int					_server;
	struct sockaddr_in	_serverAddress;
public:
	LisenStruc(size_t port);
	LisenStruc(const LisenStruc& other);
	LisenStruc & operator=(const LisenStruc& other);
	~LisenStruc();

	bool	bind();
	bool	listen();
	void	close();
	int		getServerFD();
	struct sockaddr_in & getServerAddress();
};

#endif