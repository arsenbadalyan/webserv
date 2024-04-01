#include "DirStruct.hpp"

DirStruct::DirStruct	() {}

DirStruct::DirStruct	(const std::string & name, const std::string& data, const std::string& size) \
						: _name(name), _data(data), _size(size) {}

DirStruct::DirStruct	(const DirStruct & other) : \
_name(other._name), _data(other._data), _size(other._size)	{}

DirStruct & DirStruct::operator=(const DirStruct & other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_data = other._data;
		this->_size = other._size;
	}
	return(*this);
}

std::string	DirStruct::getName()
{
	return this->_name;
}
std::string DirStruct::getData()
{
	return this->_data;
}
std::string	DirStruct::getSize()
{
	return this->_size;
}

void		DirStruct::setName(const std::string& name)
{
	this->_name = name;
}
void		DirStruct::setData(const std::string& data)
{
	this->_data = data;
}
void		DirStruct::setSize(const std::string& size)
{
	this->_size = size;
}