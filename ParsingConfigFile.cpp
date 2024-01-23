#include "ParsingConfigFile.hpp"

ParsingConfigFile::ParsingConfigFile(const std::string & filename) : _file(filename)	{}

ParsingConfigFile::ParsingConfigFile(const ParsingConfigFile & other)
{
	this->_file = other._file;
	this->_data = other._data;
}

ParsingConfigFile & ParsingConfigFile::operator=(const ParsingConfigFile & other)
{
	if (this != &other)
	{
		this->_file = other._file;
		this->_data = other._data;
	}
	return (*this);
}

ParsingConfigFile::~ParsingConfigFile() { std::cout << "Destruktor ParsingConfigFile\n";}

bool ParsingConfigFile::startPars()
{
	this->_data = this->_file.Read();
	if (!this->CheckSintex())
		throw MyException("Sintexs ERROR");
	// if (!this->CheckCoreckt())
	// 	throw MyException("Corect Error");
	return (true);
}

bool ParsingConfigFile::CheckSintex()
{
	int flag = 0;

	for (size_t i = 0; i < this->_data.size(); i++)
	{
		if(this->_data[i] == '{')
			++flag;
		if(this->_data[i] == '}')
			--flag;
		if (flag < 0)
			return (false);
	}
	if (flag != 0)
		return (false);
	return (true);
}

bool ParsingConfigFile::CheckCoreckt()
{
	
	return false;
}

/////////////////

ParsingConfigFile::MyException::MyException(const std::string &error) : _error(error) {}

ParsingConfigFile::MyException::~MyException() throw() {}

const char * ParsingConfigFile::MyException::what() const throw() { return ((this->_error.data())); }