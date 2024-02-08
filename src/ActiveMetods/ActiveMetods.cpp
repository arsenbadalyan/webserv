#include "ActiveMetods.hpp"

ActiveMetods::ActiveMetods():_post(true), _get(true), _delete(true) {}

ActiveMetods::ActiveMetods(bool post, bool get, bool del):_post(post),_get(get),_delete(del) {}

ActiveMetods::ActiveMetods(const ActiveMetods& other)
{
	this->_post = other._post;
	this->_get = other._get;
	this->_delete = other._delete;
}

ActiveMetods& ActiveMetods::operator= (const ActiveMetods& other)
{
	if (this != &other)
	{
		this->_post = other._post;
		this->_get = other._get;
		this->_delete = other._delete;
	}
	return (*this);
}

ActiveMetods::~ActiveMetods() {}

bool ActiveMetods::getPost()
{
	return (this->_post);
}

bool ActiveMetods::getGet()
{
	return (this->_get);
}

bool ActiveMetods::getDelete()
{
	return (this->_delete);
}

ActiveMetods& ActiveMetods::setPost(const bool PostData)
{
	this->_post = PostData;
	return (*this);
}

ActiveMetods& ActiveMetods::setGet(const bool GetData)
{
	this->_get = GetData;
	return (*this);
}

ActiveMetods& ActiveMetods::setDelete(const bool DeleteData)
{
	this->_delete = DeleteData;
	return (*this);
}