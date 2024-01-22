#ifndef _PARSINGCONFIGFILE_HPP_
#define _PARSINGCONFIGFILE_HPP_

#include <string>

class ParsingConfigFile {
private:

	std::string _filename;

public:

	ParsingConfigFile(std::string filename);
	ParsingConfigFile( const ParsingConfigFile & other);
	ParsingConfigFile & operator=(const ParsingConfigFile & other);
	~ParsingConfigFile();

	bool start();

private:


	class MyExeption : public std::exception
	{
	private:
		std::string _error;
	public:
		MyExeption(const std::string & error);
		~MyExeption() throw();

		const char * what() const throw();
	};

};

#endif