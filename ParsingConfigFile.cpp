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

ParsingConfigFile::~ParsingConfigFile() { }

bool ParsingConfigFile::startPars()
{
	this->_data = this->_file.Read();
	if (!this->_data.size())
		throw MyException("Emty File ERROR");
	if (!this->CheckSintex())
		throw MyException("Sintexs ERROR");
	if (!this->CheckCorecktServer())
		throw MyException("Corect Error");
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

bool ParsingConfigFile::CheckCorecktServer()
{
	std::string server = "server";

	for (size_t pos_start = 0, pos_end = 0, pos = 0; \
		pos_start < this->_data.size(); pos_start = pos_end + 1)
	{
		pos_end = this->_data.find('{', pos_start);
		if (SIZE_MAX == pos_end && pos_start == 0)
			return (false);
		if (SIZE_MAX == pos_end)
		{
			for (size_t i = pos_start; i < this->_data.size() ; ++i)
			{
				if (this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
					return (false);
			}
			return (true);
		}
		pos = this->_data.find(server.c_str(), pos_start, pos_end - pos_start);
		if (SIZE_MAX == pos_end)
			return (false);
		for (size_t i = pos_start; i < pos_end; ++i)
		{
			if (i < pos && i > pos + server.size() && \
				this->_data[i] != '\n' && this->_data[i] != '\t' && this->_data[i] != ' ')
				{
					std::cout << "i = " << i  << "  " << (int)this->_data[i] << std::endl;
					return (false);
				}
		}
		pos_start = pos_end + 1;
		pos_end = get_reng(pos_start);
		if (pos_end == SIZE_MAX)
			return (false);
		if (!CheckCoreckt(pos_start, pos_end))
			return (false);
		if (pos_end == this->_data.size())
			return (true);
	}
	return (true);
}

size_t ParsingConfigFile::get_reng(size_t start)
{
	int flag = 1;
	for ( ; start < this->_data.size(); ++start)
	{
		if (this->_data[start] == '{')
			++flag;
		if (this->_data[start] == '}')
			--flag;
		if (!flag)
			return (start);
	}
	return (SIZE_MAX);
}

bool ParsingConfigFile::CheckCoreckt(size_t pos_start, size_t pos_end)
{
	return (true);
}

/////////////////

ParsingConfigFile::MyException::MyException(const std::string &error) : _error(error) {}

ParsingConfigFile::MyException::~MyException() throw() {}

const char * ParsingConfigFile::MyException::what() const throw() { return ((this->_error.data())); }