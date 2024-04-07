#ifndef _STATUSCOLLECTION_HPP_
#define _STATUSCOLLECTION_HPP_

#include <iostream>
#include <string>
#include <vector>

#define NO_CUSTOM_STATUS_CODE -1

class StatusCollection
{
private:
	int		_statusTypes;
	std::string	_path;
public:
	StatusCollection();
	StatusCollection(const size_t &statusTypes, const std::string &path);
	StatusCollection(const StatusCollection& other);
	StatusCollection& operator=  (const StatusCollection& other);
	~StatusCollection();
	bool	isRedirection();
	int& getStatusTypes();
	std::string &getPath();
	StatusCollection& setStatusTypes(const size_t statusType);
	StatusCollection& setPath(const std::string& path);
};

#endif