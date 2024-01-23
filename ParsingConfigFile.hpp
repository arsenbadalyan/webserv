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

	bool CheckSintex();
	bool CheckCoreckt();

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