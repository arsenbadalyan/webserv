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

const bool ActiveMetods::GetPost() const
{
	return (this->_post);
}

const bool ActiveMetods::GetGet() const
{
	return (this->_get);
}

const bool ActiveMetods::GetDelete() const
{
	return (this->_delete);
}

ActiveMetods& ActiveMetods::SetPost(const bool PostData)
{
	this->_post = PostData;
	return (*this);
}

ActiveMetods& ActiveMetods::SetGet(const bool GetData)
{
	this->_post = GetData;
	return (*this);
}

ActiveMetods& ActiveMetods::SetDelete(const bool DeleteData)
{
	this->_post = DeleteData;
	return (*this);
}