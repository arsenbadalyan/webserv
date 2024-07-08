#include "FileReader.hpp"

FileReader::FileReader(){}

FileReader::FileReader(const std::string & filename)
{
	this->_filename = filename;
}

FileReader::FileReader(const std::string & filename, const std::string & result)
{
	this->_filename = filename;
	this->_result = result;
}

FileReader::FileReader(const FileReader & other)
{
	this->_filename = other._filename;
	this->_result = other._result;
}

FileReader & FileReader::operator=(const FileReader & other)
{
	if(this != &other)
	{
		this->_filename = other._filename;
		this->_result = other._result;
	}
	return (*this);
}

FileReader & FileReader::operator<<(const std::string & result)
{
	this->_result = result;
	return (*this);
}

FileReader & FileReader::operator>>(std::string & result)
{
	result = this->_result;
	return (*this);
}

FileReader::~FileReader() {}

std::string FileReader::Read()
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

FileReader & FileReader::Write()
{
	std::ofstream file(std::string("./") + _filename, std::ios_base::app);
	if (!file)
		throw MyException("no file\n");
	file << this->_result;
	file.close();
	return (*this);
}

std::string FileReader::getResult()
{
	return (this->_result);
}

FileReader & FileReader::setResult(const std::string & result)
{
	this->_result = result;
	return (*this);
}

std::string FileReader::getFilename()
{
	return (this->_filename);
}

FileReader & FileReader::setFilename(const std::string & filename)
{
	this->_filename = filename;
	return (*this);
}

FileReader & FileReader::cleanFile()
{
	std::ofstream file(std::string("./") + _filename);
	if (!file)
		throw MyException("no file\n");
	file.close();
	return (*this);
}

/////////////////

FileReader::MyException::MyException(const std::string &error) : _error(error) {}

FileReader::MyException::~MyException() throw() {}

const char *FileReader::MyException::what() const throw() { return ((this->_error.data())); }