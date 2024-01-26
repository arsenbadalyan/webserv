#ifndef _PARSINGCONFIGFILE_HPP_
#define _PARSINGCONFIGFILE_HPP_

#include "ReadFile.hpp"

class ParsingConfigFile {
private:

	// std::string	_filename;
	ReadFile	_file;
	std::string	_data;

public:

	ParsingConfigFile(const std::string & filename);
	ParsingConfigFile( const ParsingConfigFile & other);
	ParsingConfigFile & operator=(const ParsingConfigFile & other);
	~ParsingConfigFile();

	bool startPars();

private:

	bool	CheckSintex();
	bool	CheckCorecktServer();
	size_t	get_reng(size_t start);
	bool	CheckCoreckt(size_t pos_start, size_t pos_end);
	size_t	CheckCorecktConfig(std::string config, size_t pos_start, size_t pos_end);

	class MyException : public std::exception
	{
	private:
		std::string _error;
	public:
		MyException(const std::string & error);
		~MyException() throw();

		const char * what() const throw();
	};

};

#endif