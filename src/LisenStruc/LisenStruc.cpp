#include "LisenStruc.hpp"

LisenStruc::LisenStruc(size_t port)
{
    this->_serverAddress.sin_port = htons(port);
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
    // this->_serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // this->_serverAddress.sin_addr.s_addr = htons(0x7f000001);
}

LisenStruc::LisenStruc(const LisenStruc &other) :_server(other._server) , _serverAddress(other._serverAddress)
{}

LisenStruc &LisenStruc::operator=(const LisenStruc &other)
{
    this->_server = other._server;
    this->_serverAddress = other._serverAddress;
    return (*this);
}

LisenStruc::~LisenStruc()
{
}

bool LisenStruc::bind()
{
    // int option_value = 1;
    this->_server = ::socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = 30; // 30 секунд таймаут
    timeout.tv_usec = 0;

    // Установка времени таймаута для приема данных на сервере
    if (setsockopt(this->_server, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == -1) {
        std::cerr << "Failed to set receive timeout: " << strerror(errno) << std::endl;
        // close(this->_server);
        return 1;
    }
    // setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    if(this->_server < 0)
    {
        return (false);
    }
	// int opt = 1;
	// if(setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	// {
	// 	return (false);
	// }
    if (::bind(this->_server, reinterpret_cast<struct \
    sockaddr*>(&this->_serverAddress), sizeof(this->_serverAddress)) < 0)
    {
        return (false);
    }
    return true;
}

bool LisenStruc::listen()
{
    if (::listen(this->_server, 1))
		return (false);
	//new 
	// fcntl(this->_server, F_SETFL, O_NONBLOCK);
    return true;
}

void LisenStruc::close()
{
	::close(this->_server);
}

int LisenStruc::getServerFD()
{
	return this->_server;
}

sockaddr_in &LisenStruc::getServerAddress()
{
	return (this->_serverAddress);
}
