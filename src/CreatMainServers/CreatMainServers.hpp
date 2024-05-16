#ifndef _CREAT_MAIN_SERVERS_HPP_
#define _CREAT_MAIN_SERVERS_HPP_


#include "Server.hpp"
#include "RequestPool.hpp"
#include <dirent.h>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
// #include "HttpRequest.hpp"


class CreatMainServers
{
public:

	static bool startServer(std::vector<Server> & serverlist);
	static void	closeFullPorts(std::vector<Server>& serverlist);

private:
   CreatMainServers(){}
   static bool	starting(std::vector<Server> & serverlist);
   static int	bindFD(std::vector<Server> & serverlist, fd_set &rfds, fd_set &wfds);
   static void	conectClient(std::vector<Server> & serverlist, fd_set &rfds);
   static void	readClient(std::vector<Server> & serverlist, fd_set &rfds, RequestPool &requestPool);
   static void	writeClient(std::vector<Server> & serverlist, fd_set &wfds, RequestPool &requestPool);
   static void	algcat(std::vector<Server> & serverlist, int max, int &sokwrit ,int &sokread);
};


#endif