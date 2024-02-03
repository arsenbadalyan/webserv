#ifndef _READFILE_HPP_
#define _READFILE_HPP_

#include <iostream>
#include <fstream>
#include <unistd.h>

class ReadFile
{
private:

	std::string _filename;
	std::string _result;

public:

	ReadFile();
	ReadFile(const std::string & filename);
	ReadFile(const std::string & filename, const std::string & result);
	ReadFile(const ReadFile & other);

	ReadFile & operator=(const ReadFile & other);
	ReadFile & operator<<(const std::string & resilt);
	ReadFile & operator>>(std::string & result);

	~ReadFile();

	std::string Read();
	ReadFile & Write();
	std::string getResult();
	ReadFile & setResult(const std::string & result);
	std::string getFilename();
	ReadFile & setFilename(const std::string & filename);
	ReadFile & cleanFile();

private:

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