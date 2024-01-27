#include "StatusCollection.hpp"

StatusCollection::StatusCollection():_path("non") {}

StatusCollection::StatusCollection(const std::vector<int> &statusTypes, const std::string &path):_statusTypes(statusTypes),_path(path) {}


StatusCollection::StatusCollection(const StatusCollection& other):_statusTypes(other._statusTypes)
{
	this->_path = other._path;
}

StatusCollection& StatusCollection::operator= (const StatusCollection& other)
{
	if (this != &other)
	{
		this->_statusTypes = other._statusTypes;
	}
	return (*this);
}

StatusCollection::~StatusCollection()
{
	this->_statusTypes.clear();
}

std::vector<int>& StatusCollection::getStatusTypes()
{
	return (this->_statusTypes);
}

std::string& StatusCollection::getPath()
{
	return (this->_path);
}

StatusCollection& StatusCollection::setStatusTypes(const int statusType)
{
	this->_statusTypes.push_back(statusType);
	return (*this);
}

StatusCollection& StatusCollection::setPath(const std::string& path)
{
	this->_path = path;
	return (*this);
}