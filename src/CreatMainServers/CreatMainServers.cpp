#include "CreatMainServers.hpp"

#define BUFFER_SIZE 6000
#define COUNT_CHEACK 15

bool CreatMainServers::startServer(std::vector<Server> & serverlist)
{
	if (!CreatMainServers::starting(serverlist))
	{
		return (false);
	}
	struct timeval  tv;
	fd_set	rfds;
	fd_set	wfds;
	fd_set	efds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	// std::string q;
	// while (q != "#")
	while(1)
	{
		int a = CreatMainServers::bindFD(serverlist, rfds, wfds);
		// ---------------------------------
		int res = select(a + 1, &rfds, &wfds, &efds, &tv );
		// ----------------------------------
		// std::cout << "selecti tvat res = " << res << std::endl;
		if (res >= 0)
		{
			CreatMainServers::writeClient(serverlist, wfds);
			CreatMainServers::readClient(serverlist, rfds);
			CreatMainServers::conectClient(serverlist, rfds);
		}
		// std::cin >> q;
		// std::cout << "continue...\n";
	}
	return true;
}

void CreatMainServers::closeFullPorts(std::vector<Server> &serverlist)
{
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		{
			serverlist[j].getMainServer()[i].close();
		}
		
	}
}

bool CreatMainServers::starting(std::vector<Server> &serverlist)
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

int	CreatMainServers::bindFD(std::vector<Server> &serverlist, fd_set &rfds, fd_set &wfds)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	int a = 3;
	int sokwrit = 0;
	int OLDsokwrit = 0;
	int sokread = 0;
	//serveri diskriptrner kardalu hamar
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		{
			// std::cout << "meyn server ds = " << serverlist[j].getMainServer()[i].getServerFD() << std::endl;
			if (serverlist[j].getMainServer()[i].getServerFD() > a)
				a = serverlist[j].getMainServer()[i].getServerFD();
			FD_SET(serverlist[j].getMainServer()[i].getServerFD(), &rfds);
		}
		
	}
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		sokread += serverlist[j].getReadSocket().size();
		sokwrit += serverlist[j].getWritSocket().size();
	}
	OLDsokwrit = sokwrit;
	if (FD_SETSIZE - a < sokread + sokwrit)
	{
		sokread = sokread / 2 ;
		sokwrit = FD_SETSIZE - a - sokread;
		if (sokwrit > OLDsokwrit)
		{
			sokwrit = OLDsokwrit;
			sokread = FD_SETSIZE - a - sokread;
		}
	}
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getWritSocket().size() && i * j <= sokwrit; i++)
		{
			// std::cout << "Client write  ds = " << serverlist[j].getClientSocket()[i] << std::endl;
			if (serverlist[j].getWritSocket()[i] > a)
				a = serverlist[j].getWritSocket()[i];
			FD_SET(serverlist[j].getWritSocket()[i], &wfds);
		}
		
	}
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getReadSocket().size() && i * j <= sokread; i++)
		{
			// std::cout << "client read ds = " << serverlist[j].getServerSocket()[i] << std::endl;
			if ((serverlist[j].getReadSocket()[i])._fd > a)
				a = (serverlist[j].getReadSocket()[i])._fd;
			FD_SET((serverlist[j].getReadSocket()[i])._fd, &rfds);
		}
		
	}
	return a;
}

void CreatMainServers::conectClient(std::vector<Server> &serverlist, fd_set &rfds)
{
	//stugum em serveri fd kardalu hamar
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		{
			// std::cout << "ERROR 1 \n";
			// std::cout << "j == " << j << " i == " << i << " diskrip = " << serverlist[j].getMainServer()[i].getServerFD() << std::endl;
			// std::cout << "grelu hamar servri port hamar -> " << serverlist[j].getPort()[i] << 
			// " == " << FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds) << std::endl;
			if (FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds))
			{
				// std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaa_1\n";
				socklen_t siz = sizeof(serverlist[j].getMainServer()[i].getServerAddress());
				serverlist[j].getReadSocket().push_back(Server::ReadSoket(accept(serverlist[j].getMainServer()[i].getServerFD(), \
				reinterpret_cast<struct sockaddr*>(&(serverlist[j].getMainServer()[i].getServerAddress())), &siz), 1));
				// std::cout << "serveri tvat darnuma client fd = " << (serverlist[j].getServerSocket()[serverlist[j].getServerSocket().size() - 1]) << std::endl;
				// std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaa_2\n";

				if ((serverlist[j].getReadSocket()[serverlist[j].getReadSocket().size() - 1])._fd < 0)
				{
					serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + serverlist[j].getReadSocket().size() - 1);
					std::cout << "ERROR Client 444\n";
				}
			}
		}
	}
}

void CreatMainServers::readClient(std::vector<Server> &serverlist, fd_set &rfds)
{
	char buffer[BUFFER_SIZE];
	//stuguma serveri diskriptor kardalu hamar
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getReadSocket().size(); i++)
		{
			// std::cout << i <<"aaaaaa____1\n";
			// std::cout << "te Server kardalu hamar patrasta kardalu hamar -> " << serverlist[j].getServerSocket()[i] << 
			// " == " << FD_ISSET(serverlist[j].getServerSocket()[i], &rfds) << std::endl;
			if(FD_ISSET(serverlist[j].getReadSocket()[i]._fd, &rfds))
			{
				// std::cout << i <<"bbbbbb____1\n";
				bzero(buffer,BUFFER_SIZE);
				// std::cout << "aaaaaa____1\n";
				// std::cout << "diskrip blok = " << serverlist[j].getServerSocket()[i] << std::endl;

				// int aaa =
				recv(serverlist[j].getReadSocket()[i]._fd, buffer, BUFFER_SIZE ,0);

				// std::cout << "aaaaaa____2\n";
				// std::cout << "serveri kardatatn soketic = " << aaa << std::endl; 
				// std::cout << "aaaaaa____3\n";
				std::cout << buffer << std::endl;

				serverlist[j].getWritSocket().push_back(serverlist[j].getReadSocket()[i]._fd);
				serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + i);
			}
			else{
				if(serverlist[j].getReadSocket()[i]._flag >= COUNT_CHEACK)
				{
					close(serverlist[j].getReadSocket()[i]._fd);
					serverlist[j].getReadSocket().erase(serverlist[j].getReadSocket().begin() + i);
				}
				else{
					++(serverlist[j].getReadSocket()[i]._flag);
				}
			}
		}
		
	}
}

void CreatMainServers::writeClient(std::vector<Server> &serverlist, fd_set &wfds)
{
	// stugum em clienti fd grelu hamar
	char buffer[BUFFER_SIZE];
	for (size_t j = 0; j < serverlist.size(); j++)
	{
		for (size_t i = 0; i < serverlist[j].getWritSocket().size(); i++)
		{
			// std::cout << "te client patrasta grelu hamar -> " << serverlist[j].getClientSocket()[i] << 
			// " == " << FD_ISSET(serverlist[j].getClientSocket()[i], &wfds) << std::endl;
			// std::cout << i <<"aaaaaa____0\n";
			if(FD_ISSET(serverlist[j].getWritSocket()[i], &wfds))
			{
				// std::cout << i <<"bbbbbb____0\n";
				int file = open("index.html", O_RDONLY);
				bzero(buffer,BUFFER_SIZE);
				if (file >= 0)
				{
					read(file, buffer, BUFFER_SIZE);
					std::string resp;
					resp += buffer;

					// int aaa = write(serverlist[j].getClientSocket()[i], (void*)(resp.c_str()), resp.size());
					// if (aaa < 0)
					// {
					// 	std::cout << "error write \n";
					// 	std::string ttt;
					// 	std::cin >> ttt;
					// 	(void)ttt;
					// }

					// std::cout << resp;
					// std::cout << "serveri grat soketum send = " << 
					send(serverlist[j].getWritSocket()[i], resp.c_str(), resp.size(), 0);
					//  << std::endl;
					// aaa << std::endl;
					close(file);
				}
				//uxarkum u jnjuma fd
			}
			close(serverlist[j].getWritSocket()[i]);
			serverlist[j].getWritSocket().erase(serverlist[j].getWritSocket().begin() + i);
		}
		
	}
}
