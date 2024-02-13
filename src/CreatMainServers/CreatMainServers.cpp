#include "CreatMainServers.hpp"

#define BUFFER_SIZE 2048

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
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
			{
				FD_SET(serverlist[j].getMainServer()[i].getServerFD(), &rfds);
			}
			
		}
		int res = select(50, &rfds, &wfds, &efds, &tv );
		std::cout << "res = " << res << std::endl;
		for (size_t j = 0; j < serverlist.size(); j++)
		{
			for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
			{
				std::cout << "ERROR 1 \n";
				std::cout << serverlist[j].getPort()[i] << " ==" << std::endl;//FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds) << std::endl;
				if (FD_ISSET(serverlist[j].getMainServer()[i].getServerFD(), &rfds))
				{
				    socklen_t siz = sizeof(serverlist[j].getMainServer()[i].getServerAddress());
					serverlist[j].getClientSocket().push_back(accept(serverlist[j].getMainServer()[i].getServerFD(), \
					reinterpret_cast<struct sockaddr*>(&(serverlist[j].getMainServer()[i].getServerAddress())), &siz));
					std::cout << "fd = " << (serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]) << std::endl;
					if ((serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]) < 0)
					{
						serverlist[j].getClientSocket().erase(serverlist[j].getClientSocket().begin() + serverlist[j].getClientSocket().size() - 1);
						std::cout << "ERROR Client 3\n";
					}
					else{
						bzero(buffer,BUFFER_SIZE);
						recv(serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1], buffer, BUFFER_SIZE,0);
						std::cout << buffer << std::endl;
						int file = open("index.html", O_RDONLY);
						bzero(buffer,BUFFER_SIZE);
						if (file >= 0)
						{
							// bzero(buffer,BUFFER_SIZE);
							int aaa = read(file, buffer, BUFFER_SIZE);
							std::cout << "aaaaaaaaa = " << aaa << std::endl; 
							// (void)aaa;
							std::string resp;
							resp += buffer;
							// std::cout << resp;
							std::cout << "send = " << send((serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]), \
							&resp, resp.size(), 0) << std::endl;
							close(file);
						}
						// close(serverlist[j].getClientSocket()[serverlist[j].getClientSocket().size() - 1]);
						// serverlist[j].getClientSocket().erase(serverlist[j].getClientSocket().begin() + serverlist[j].getClientSocket().size() - 1);
					}
				}
			}
		}
		// for (size_t j = 0; j < serverlist.size(); j++)
		// {
		// 	for (size_t i = 0; i < serverlist[j].getMainServer().size(); i++)
		// 	{
		// 		if(false)
		// 		{
		// 			int file = open("index.html", O_RDONLY);
		// 			if (file >= 0)
		// 			{
		// 				bzero(buffer,BUFFER_SIZE);
		// 				int aaa = read(file, buffer, BUFFER_SIZE);
		// 				(void)aaa;
		// 				std::string resp;
		// 				resp += buffer;
		// 				std::cout << "send = " << send((serverlist[j].getMainServer()[i].getServerFD()), &resp, resp.size(), 0) << std::endl;
		// 				close(file);

		// 			}
		// 		}
		// 	}
			
		// }
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
