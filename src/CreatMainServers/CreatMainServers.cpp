#include "CreatMainServers.hpp"

bool CreatMainServers::startServer(std::vector<Server> & serverlist)
{
    (void)serverlist;
    return false;
}

void CreatMainServers::starting(std::vector<Server> &serverlist)
{
    for (size_t j = 0; j < serverlist.size(); ++j)
    {
        for (size_t i = 0; i < serverlist[j].getPort().size(); i++)
        {
            /* code */
        }
        
    }
    
}
