#include "ActiveMetods.hpp"

const std::string SupportedMethods::POST = "post";
const std::string SupportedMethods::GET = "get";
const std::string SupportedMethods::DELETE = "delete";

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

bool ActiveMetods::isAvailableMethod(std::string methodName) {
	methodName = Util::toLower(methodName);

	std::cout << "POST: " << this->getPost() << std::endl;
	std::cout << "GET: " << this->getGet() << std::endl;
	std::cout << "DELETE: " << this->getDelete() << std::endl;

	if (RootConfigs::AvailableMethods.find(methodName) == RootConfigs::AvailableMethods.end())
		return (false);

	if (methodName == SupportedMethods::POST)
		return (this->getPost());

	if (methodName == SupportedMethods::GET)
		return (this->getGet());

	if (methodName == SupportedMethods::DELETE)
		return (this->getDelete());

	return (false);
}