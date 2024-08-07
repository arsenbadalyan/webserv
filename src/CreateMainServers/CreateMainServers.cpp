#include "CreateMainServers.hpp"

#define BUFFER_SIZE 10000
#define COUNT_CHEACK 150

bool CreateMainServers::startServer(std::vector<Server> & serverlist)
{
	if (!CreateMainServers::starting(serverlist))
	{
		return (false);
	}
	// struct timeval  tv;
	RequestPool *requestPool = new RequestPool();
	fd_set	rfds, wfds;

	// tv.tv_sec = 0;
	// tv.tv_usec = 0;
	while(1)
	{
		int max = CreateMainServers::bindFD(serverlist, rfds, wfds);

		::logRequest(LOGGER_DEBUG, "Waiting for new connection...");
		int res = select(max + 1, &rfds, &wfds, 0, 0);

		if (res >= 0)
		{
			CreateMainServers::writeClient(serverlist, wfds, *requestPool);
			CreateMainServers::readClient(serverlist, rfds, *requestPool);
			CreateMainServers::conectClient(serverlist, rfds);
		}
	}
	return true;
}

void CreateMainServers::closeFullPorts(std::vector<Server> * serverlist)
{
	// for (size_t j = 0; j < serverlist.size(); j++)
	// {
	// 	for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
	// 	{
	// 		serverlist[j].getMainServer()[i].close();
	// 	}
	// }
	for (size_t j = 0; j < serverlist->size(); j++)
	{
		for (size_t i = 0; i < (*serverlist)[j].getMainServer().size(); i++)
		{
			// std::cout << "close ports i = " << i << std::endl;
			(*serverlist)[j].getMainServer()[i].close();
		}
	}
}

bool CreateMainServers::starting(std::vector<Server> &serverlist)
{
	bool f = false;
    for (size_t j = 0 ; j < serverlist.size(); f ? f = false : ++j)
    {
		for (size_t i = 0; i < serverlist[j].getPort().size(); i++)
		{
			serverlist[j].getMainServer().push_back(serverlist[j].getPort()[i]);
		}
        for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
        {
            if (!(serverlist[j].getMainServer()[i].bind()) || !(serverlist[j].getMainServer()[i].listen()))
			{
				f = true;
				for (size_t l = 0; l <= i; l++)
				{
					serverlist[j].getMainServer()[i].close();
				}
				std::cerr << "Error Server " << serverlist[j].getServerName() << " Port " << serverlist[j].getPort()[i] << std::endl;
				serverlist.erase(serverlist.begin() + j);
				break;
			}
        }
        
    }
	if (serverlist.size())
		return (true);
    return false;
    
}

int	CreateMainServers::bindFD(std::vector<Server> &serverlist, fd_set &rfds, fd_set &wfds)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	int max = 3;
	int sokwrit = 0;
	int sokread = 0;
	//serveri diskriptrner kardalu hamar
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		{
			if (serverlist[j].getMainServer()[i].getServerFD() > max)
				max = serverlist[j].getMainServer()[i].getServerFD();
			FD_SET(serverlist[j].getMainServer()[i].getServerFD(), &rfds);
		}
	}
	algcat(serverlist, max, sokwrit, sokread);
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getWritSocket().size() && static_cast<int>(i * j) <= sokwrit; i++)
		{
			if (serverlist[j].getWritSocket()[i] > max)
				max = serverlist[j].getWritSocket()[i];
			FD_SET(serverlist[j].getWritSocket()[i], &wfds);
		}
		
	}
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getReadSocket().size() && static_cast<int>(i * j) <= sokread; i++)
		{
			if ((serverlist[j].getReadSocket()[i])._fd > max)
				max = (serverlist[j].getReadSocket()[i])._fd;
			FD_SET((serverlist[j].getReadSocket()[i])._fd, &rfds);
		}
		
	}
	return max;
}

void CreateMainServers::conectClient(std::vector<Server> &serverlist, fd_set &rfds)
{
	//stugum em serveri fd kardalu hamar
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		{
			if (FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds))
			{
				socklen_t siz = sizeof(serverlist[j].getMainServer()[i].getServerAddress());
				serverlist[j].getReadSocket().push_back(Server::ReadSoket(accept(serverlist[j].getMainServer()[i].getServerFD(), \
				reinterpret_cast<struct sockaddr*>(&(serverlist[j].getMainServer()[i].getServerAddress())), &siz), 1));
				// struct timeval timeout;
				// timeout.tv_sec = 30; // 30 секунд таймаут
				// timeout.tv_usec = 0;

				// // Установка времени таймаута для приема данных на сервере
				// if (setsockopt((serverlist[j].getReadSocket()[serverlist[j].getReadSocket().size() - 1])._fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == -1) {
				// 	std::cerr << "Failed to set receive timeout: " << strerror(errno) << std::endl;
				// 	// close(this->_server);
				// }
				if ((serverlist[j].getReadSocket()[serverlist[j].getReadSocket().size() - 1])._fd < 0)
				{
					serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + serverlist[j].getReadSocket().size() - 1);
					std::cerr << "ERROR Client 444\n";
				}
				// else{
				// 	//new 
				// 	fcntl((serverlist[j].getReadSocket()[serverlist[j].getReadSocket().size() - 1])._fd, F_SETFL, O_NONBLOCK);
				// }

			}
		}
	}
}

void CreateMainServers::readClient(std::vector<Server> &serverlist, fd_set &rfds, RequestPool & requestPool)
{
	// char buffer[BUFFER_SIZE];
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getReadSocket().size(); i++)
		{
			if(FD_ISSET(serverlist[j].getReadSocket()[i]._fd, &rfds))
			{
				///// sra tex talisa funqcain kardalu hamar <----------------------------------
				bool needsToClose = requestPool.sendRequest(serverlist[j].getReadSocket()[i]._fd, serverlist[j]);
				////
				if (needsToClose) {
					serverlist[j].getWritSocket().push_back(serverlist[j].getReadSocket()[i]._fd);
					serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + i);
				}
			}
			else
			{
				if(serverlist[j].getReadSocket()[i]._flag >= COUNT_CHEACK)
				{
					close(serverlist[j].getReadSocket()[i]._fd);
					serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + i);
				}
				else
				{
					++(serverlist[j].getReadSocket()[i]._flag);
				}
			}
		}
	}
}

void CreateMainServers::writeClient(std::vector<Server> &serverlist, fd_set &wfds, RequestPool & requestPool)
{
	// stugum em clienti fd grelu hamar
	// char buffer[BUFFER_SIZE];
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getWritSocket().size(); i++)
		{
			if(FD_ISSET(serverlist[j].getWritSocket()[i], &wfds))
			{
				///// sra tex talisa funqcain grelu hamar <-------------------------------
				requestPool.getResponse(serverlist[j].getWritSocket()[i]);
				requestPool.destroyRequest(serverlist[j].getWritSocket()[i]);
				// int file = open("index.html", O_RDONLY);
				// bzero(buffer,BUFFER_SIZE);
				// if (file >= 0)
				// {
				// 	read(file, buffer, BUFFER_SIZE);
				// 	std::string resp;
				// 	resp += buffer;
				// 	send(serverlist[j].getWritSocket()[i], resp.c_str(), resp.size(), 0);
				// 	close(file);
				// }
				//////
			}
			close(serverlist[j].getWritSocket()[i]);
			serverlist[j].getWritSocket().erase(serverlist[j].getWritSocket().begin() + i);
		}
	}
}

void CreateMainServers::algcat(std::vector<Server> &serverlist, int max, int &sokwrit, int &sokread)
{
	int OLDsokwrit = 0;
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		sokread += serverlist[j].getReadSocket().size();
		sokwrit += serverlist[j].getWritSocket().size();
	}
	OLDsokwrit = sokwrit;
	if (FD_SETSIZE - max < sokread + sokwrit)
	{
		sokread = sokread / 2 ;
		sokwrit = FD_SETSIZE - max - sokread;
		if (sokwrit > OLDsokwrit)
		{
			sokwrit = OLDsokwrit;
			sokread = FD_SETSIZE - max - sokread;
		}
	}
}
