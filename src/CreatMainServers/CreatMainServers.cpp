#include "CreatMainServers.hpp"

#define BUFFER_SIZE 6000

bool CreatMainServers::startServer(std::vector<Server> & serverlist)
{
	char buffer[BUFFER_SIZE];
	if (!CreatMainServers::starting(serverlist))
	{
		return (false);
	}
	struct timeval  tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	fd_set	rfds;
	fd_set	wfds;
	fd_set	efds;
	std::string q;
	while (q != "#")
	{
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		int a = 3;
		//serveri diskriptrner kardalu hamar
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
			{
				if (j == 0 && i == 0)
					continue;
				if (serverlist[j].getMainServer()[i].getServerFD() > a)
					a = serverlist[j].getMainServer()[i].getServerFD();
				FD_SET(serverlist[j].getMainServer()[i].getServerFD(), &rfds);
			}
			
		}
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getClientSocket().size(); i++)
			{
				if (serverlist[j].getClientSocket()[i] > a)
					a = serverlist[j].getClientSocket()[i];
				FD_SET(serverlist[j].getClientSocket()[i], &wfds);
			}
			
		}

		// ---------------------------------
		int res = select(a + 1, &rfds, &wfds, &efds, &tv );
		// ----------------------------------
		std::cout << "selecti tvat res = " << res << std::endl;


		// stugum em clienti fd grelu hamar
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getClientSocket().size(); i++)
			{
				std::cout << "te client patrasta grelu hamar -> " << serverlist[j].getClientSocket()[i] << \
				" == " << FD_ISSET(serverlist[j].getClientSocket()[i], &wfds) << std::endl;
				if(FD_ISSET(serverlist[j].getClientSocket()[i], &wfds))
				{
					int file = open("index.html", O_RDONLY);
					bzero(buffer,BUFFER_SIZE);
					if (file >= 0)
					{
						read(file, buffer, BUFFER_SIZE);
						std::string resp;
						resp += buffer;
						// std::cout << resp;
						std::cout << "serveri grat soketum send = " << \
						send(serverlist[j].getClientSocket()[i], resp.c_str(), resp.size(), 0) << std::endl;
						close(file);
					}
					//uxarkum u jnjuma fd
					close(serverlist[j].getClientSocket()[i]);
					serverlist[j].getClientSocket().erase(serverlist[j].getClientSocket().begin() + i);
				}
			}
			
		}
		//stugum em serveri fd kardalu hamar
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
			{
				// std::cout << "ERROR 1 \n";
				std::cout << "j == " << j << " i == " << i << " diskrip = " << serverlist[j].getMainServer()[i].getServerFD() << std::endl;
				std::cout << "grelu hamar servri port hamar -> " << serverlist[j].getPort()[i] << \
				" == " << FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds) << std::endl;
				if (FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds))
				{
				    socklen_t siz = sizeof(serverlist[j].getMainServer()[i].getServerAddress());
					serverlist[j].getClientSocket().push_back(accept(serverlist[j].getMainServer()[i].getServerFD(), \
					reinterpret_cast<struct sockaddr*>(&(serverlist[j].getMainServer()[i].getServerAddress())), &siz));
					std::cout << "serveri tvat darnuma client fd = " << (serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]) << std::endl;
					if ((serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]) < 0)
					{
						serverlist[j].getClientSocket().erase(serverlist[j].getClientSocket().begin() + serverlist[j].getClientSocket().size() - 1);
						std::cout << "ERROR Client 444\n";
					}
					else{
						bzero(buffer,BUFFER_SIZE);
						std::cout << "aaaaaa____1\n";
						std::cout << "diskrip blok = " << serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1] << std::endl;
						int aaa = recv(serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1], buffer, BUFFER_SIZE,0);
						std::cout << "aaaaaa____2\n";
						std::cout << "serveri kardatatn soketic = " << aaa << std::endl; 
						std::cout << "aaaaaa____3\n";
						std::cout << buffer << std::endl;


						// int file = open("index.html", O_RDONLY);
						// bzero(buffer,BUFFER_SIZE);
						// if (file >= 0)
						// {
						// 	read(file, buffer, BUFFER_SIZE);
						// 	std::string resp;
						// 	resp += buffer;
						// 	// std::cout << resp;
						// 	std::cout << "serveri grat soketum send = " << \
						// 	send(serverlist[j].getClientSocket()[i], resp.c_str(), resp.size(), 0) << std::endl;
						// 	close(file);
						// }
						// //uxarkum u jnjuma fd
						// close(serverlist[j].getClientSocket()[i]);
						// serverlist[j].getClientSocket().erase(serverlist[j].getClientSocket().begin() + i);

					}
				}
			}
		}
		std::cin >> q;
		std::cout << "continue...\n";
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
