#include "ParsingConfigFile.hpp"
void print_struct(std::vector<Server>  s);

int main() {

	std::vector<Server> t;
	ParsingConfigFile  a("file");
	try
	{
		t = a.startPars();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	print_struct(t);
	
    return (0);
}

void print_struct(std::vector<Server>  s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        std::cout << "Server " << i << " Host = " << s[i].getHost() << std::endl;
        std::cout << "Server " << i << " Port = " << s[i].getPort() << std::endl;
        std::cout << "Server " << i << " ServerName = " << s[i].getServerName() << std::endl;
        std::cout << "Server " << i << " getAllow_methods = " << s[i].getServerConfig().getAllow_methods().getGet() \
        << " " << s[i].getServerConfig().getAllow_methods().getPost() << " " << \
        s[i].getServerConfig().getAllow_methods().getDelete() << std::endl;
        std::cout << "Server " << i << " Autoindex = " << s[i].getServerConfig().getAutoindex() << std::endl;
        std::cout << "Server " << i << " CGI = " << s[i].getServerConfig().getCgi() << std::endl;
        std::cout << "Server " << i << " max_body_size = " << s[i].getServerConfig().getClient_max_body_size() << std::endl;
        for (std::map<int, std::string>::iterator h = s[i].getServerConfig().getError_page().begin();\
         h != s[i].getServerConfig().getError_page().end(); h++)
        {
            std::cout << "Server " << i << " Error_page = " << h->first <<" "<< h->second << std::endl;
        }
        for (size_t h = 0; h < s[i].getServerConfig().getIndex().size(); h++)
        {
            std::cout << "Server " << i << " Index = " << s[i].getServerConfig().getIndex()[h] << std::endl;
        }
        std::cout << "Server " << i << " Location_name = " << s[i].getServerConfig().getLocation_name() << std::endl;
        std::cout << "Server " << i << " Return = " << s[i].getServerConfig().getReturn().getStatusTypes()\
        << " " << s[i].getServerConfig().getReturn().getPath() << std::endl;
        std::cout << "Server " << i << " Root = " << s[i].getServerConfig().getRoot() << std::endl;
        std::cout << "Server " << i << " Upload_dir = " << s[i].getServerConfig().getUpload_dir() << std::endl;
        for (size_t j = 0; j < s[i].getLocations().size(); j++)
        {
            std::cout << "  Lokacins " << j << " "<< std::endl;
            std::cout << "  Server " << i << " getAllow_methods = " << s[i].getLocations()[j].getAllow_methods().getGet() \
            << " " << s[i].getLocations()[j].getAllow_methods().getPost() << " " << \
            s[i].getLocations()[j].getAllow_methods().getDelete() << std::endl;
            std::cout << "  Server " << i << " Autoindex = " << s[i].getLocations()[j].getAutoindex() << std::endl;
            std::cout << "  Server " << i << " CGI = " << s[i].getLocations()[j].getCgi() << std::endl;
            std::cout << "  Server " << i << " max_body_size = " << s[i].getLocations()[j].getClient_max_body_size() << std::endl;
            for (std::map<int, std::string>::iterator h = s[i].getLocations()[j].getError_page().begin();\
            h != s[i].getLocations()[j].getError_page().end(); h++)
            {
                std::cout << "  Server " << i << " Error_page = " << h->first <<" "<< h->second << std::endl;
            }
            for (size_t h = 0; h < s[i].getLocations()[j].getIndex().size(); h++)
            {
                std::cout << "  Server " << i << " Index = " << s[i].getLocations()[j].getIndex()[h] << std::endl;
            }
            std::cout << "  Server " << i << " Location_name = " << s[i].getLocations()[j].getLocation_name() << std::endl;
            std::cout << "  Server " << i << " Return = " << s[i].getLocations()[j].getReturn().getStatusTypes()\
            << " " << s[i].getLocations()[j].getReturn().getPath() << std::endl;
            std::cout << "  Server " << i << " Root = " << s[i].getLocations()[j].getRoot() << std::endl;
            std::cout << "  Server " << i << " Upload_dir = " << s[i].getLocations()[j].getUpload_dir() << std::endl;

        }
        
    }
    
}