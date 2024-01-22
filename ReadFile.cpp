#include "ReadFile.hpp"

ReadFile::ReadFile(){}
 
ReadFile::ReadFile(const std::string & filename)
{
	this->_filename = filename;
}

ReadFile::ReadFile(const std::string & filename, const std::string & result)
{
	this->_filename = filename;
	this->_result = result;
}

ReadFile::ReadFile(const ReadFile & other)
{
	this->_filename = other._filename;
	this->_result = other._result;
}

ReadFile & ReadFile::operator=(const ReadFile & other)
{
	if(this != &other)
	{
		this->_filename = other._filename;
		this->_result = other._result;
	}
	return (*this);
}

ReadFile & ReadFile::operator<<(const std::string & result)
{
	this->_result = result;
	return (*this);
}

ReadFile & ReadFile::operator>>(std::string & result)
{
	result = this->_result;
	return (*this);
}

ReadFile::~ReadFile() {}

std::string ReadFile::Read()
{
	this->_result.clear();
	std::ifstream file(std::string("./") + _filename);
	if (!file)
		throw MyException("no file\n");
	std::string cp;
	while(!file.eof())
	{
		cp.clear();
		cp = file.get();
		if (-1 != (int)cp[0])
			this->_result += cp;
	}
	file.close();
	return (this->_result);
}

ReadFile & ReadFile::Write()
{
	std::ofstream file(std::string("./") + _filename, std::ios_base::app);
	if (!file)
		throw MyException("no file\n");
	file << this->_result;
	file.close();
	return (*this);
}

std::string ReadFile::getResult()
{
	return (this->_result);
}

ReadFile & ReadFile::setResult(const std::string & result)
{
	this->_result = result;
	return (*this);
}

std::string ReadFile::getFilename()
{
	return (this->_filename);
}

ReadFile & ReadFile::setFilename(const std::string & filename)
{
	this->_filename = filename;
	return (*this);
}

ReadFile & ReadFile::cleanFile()
{
	std::ofstream file(std::string("./") + _filename);
	if (!file)
		throw MyException("no file\n");
	file.close();
	return (*this);
}

/////////////////

ReadFile::MyException::MyException(const std::string &error) : _error(error) {}

ReadFile::MyException::~MyException() throw() {}

const char *ReadFile::MyException::what() const throw() { return ((this->_error.data())); }