#ifndef _SETUPSERVER_HPP_
#define _SETUPSERVER_HPP_

#include <iostream>
#include <vector>
#include "Server.hpp"

#define ROOT "root"
#define ALLOW_METHODS "allow_methods"
#define AULOAD_DIR "upload_dir"
#define ERROR_PAGE "error_page"
#define RETURN "return"
#define CGI "cgi"
#define INDEX "index"
#define AUTOINDEX "autoindex"
#define BODY_SIZE "client_max_body_size"
#define LOCATION "location"
#define SERVER "server"

class SetUpServer
{
private:
	std::vector<Server> _servec;
public:
};

#endif