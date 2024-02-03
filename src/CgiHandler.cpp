#include "CgiHandler.hpp"

CgiHandler::CgiHandler():_php("none"),_python("none") {}

CgiHandler::CgiHandler(std::string php, std::string python):_php(php),_python(python) {}

CgiHandler::CgiHandler(const CgiHandler& other)
{
	this->_php = other._php;
	this->_python = other._python;
}

CgiHandler& CgiHandler::operator= (const CgiHandler& other)
{
	if (this != &other)
	{
		this->_php = other._php;
		this->_python = other._python;
	}
	return (*this);
}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::getPhp() const
{
	return (this->_php);
}

std::string CgiHandler:: getPython() const
{
	return (this->_python);
}

CgiHandler& CgiHandler::setPhp(const std::string php)
{
	this->_php = php;
	return (*this);
}

CgiHandler& CgiHandler::setPython(const std::string python)
{
	this->_python = python;
	return (*this);
}