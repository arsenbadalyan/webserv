#ifndef _CREAT_MAIN_SERVERS_HPP_
#define _CREAT_MAIN_SERVERS_HPP_


#include "Server.hpp"
#include <dirent.h>
#include <sstream>
#include <cstdint>
#include <algorithm>

class CreatMainServers
{
public:

   static bool startServer(std::vector<Server> & serverlist);

private:
   CreatMainServers(){}
   static void starting(std::vector<Server> & serverlist);
};


#endif