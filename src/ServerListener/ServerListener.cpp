#include "ServerListener.hpp"

ServerListener::ServerListener(size_t port, std::string host)
{
    this->_serverAddress.sin_port = htons(port);
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
    // this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
}

ServerListener::ServerListener(const ServerListener &other) :_server(other._server) , _serverAddress(other._serverAddress)
{}

ServerListener &ServerListener::operator=(const ServerListener &other)
{
    this->_server = other._server;
    this->_serverAddress = other._serverAddress;
    return (*this);
}

ServerListener::~ServerListener()
{
}

bool ServerListener::bind()
{
    int option_value = 1;
    this->_server = ::socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    if(this->_server < 0)
    {
        return (false);
    }

    if (::bind(this->_server, reinterpret_cast<struct \
    sockaddr*>(&this->_serverAddress), sizeof(this->_serverAddress)) < 0)
    {
        return (false);
    }
    return true;
}

bool ServerListener::listen()
{
    if (::listen(this->_server, 1))
		return (false);
    return (true);
}

void ServerListener::close()
{
	::close(this->_server);
}

int ServerListener::getServerFD()
{
	return this->_server;
}

sockaddr_in &ServerListener::getServerAddress()
{
	return (this->_serverAddress);
}
