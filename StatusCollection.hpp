#ifndef _STATUSCOLLECTION_HPP_
#define _STATUSCOLLECTION_HPP_

#include <iostream>
#include <string>
#include <vector>

class StatusCollection
{
private:
	std::vector<int> _statusTypes;
	std::string _path;
public:
	StatusCollection();
	StatusCollection(const std::vector<int> &statusTypes, const std::string &path);
	StatusCollection(const StatusCollection& other);
	StatusCollection& operator=  (const StatusCollection& other);
	~StatusCollection();
	std::vector<int>& getStatusTypes();
	std::string &getPath();
	StatusCollection& setStatusTypes(const int statusType);
	StatusCollection& setPath(const std::string& path);
};

#endif