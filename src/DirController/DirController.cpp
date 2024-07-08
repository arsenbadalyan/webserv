#include "DirController.hpp"

DirController::DirController	() {}

DirController::~DirController() {}

DirController::DirController	(const std::string & name, const std::string & href, const std::string& data, const std::string& size) \
						: _name(name), _href(href), _data(data), _size(size) {}

DirController::DirController	(const DirController & other) : \
_name(other._name), _href(other._href), _data(other._data), _size(other._size)	{}

DirController & DirController::operator=(const DirController & other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_data = other._data;
		this->_size = other._size;
	}
	return(*this);
}

std::string	DirController::getName()
{
	return this->_name;
}
std::string	DirController::getHref()
{
	return this->_href;
}
std::string & DirController::getData()
{
	return this->_data;
}
std::string	DirController::getSize()
{
	return this->_size;
}

void		DirController::setName(const std::string& name)
{
	this->_name = name;
}
void		DirController::setHref(const std::string& href)
{
	this->_href = href;
}
void		DirController::setData(const std::string& data)
{
	this->_data = data;
}
void		DirController::setSize(const std::string& size)
{
	this->_size = size;
}