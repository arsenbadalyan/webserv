#ifndef _CREAT_MAIN_SERVERS_HPP_
#define _CREAT_MAIN_SERVERS_HPP_


#include "Server.hpp"
#include <dirent.h>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <stdio.h>

class CreatMainServers
{
public:

	static bool startServer(std::vector<Server> & serverlist);
	static void	closeFullPorts(std::vector<Server> &serverlist);

private:
   CreatMainServers(){}
   static bool starting(std::vector<Server> & serverlist);
};


#endif