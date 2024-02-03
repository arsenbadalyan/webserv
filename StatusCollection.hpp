#ifndef _STATUSCOLLECTION_HPP_
#define _STATUSCOLLECTION_HPP_

#include <iostream>
#include <string>
#include <vector>

class StatusCollection
{
private:
	size_t		_statusTypes;
	std::string	_path;
public:
	StatusCollection();
	StatusCollection(const size_t &statusTypes, const std::string &path);
	StatusCollection(const StatusCollection& other);
	StatusCollection& operator=  (const StatusCollection& other);
	~StatusCollection();
	size_t& getStatusTypes();
	std::string &getPath();
	StatusCollection& setStatusTypes(const size_t statusType);
	StatusCollection& setPath(const std::string& path);
};

#endif