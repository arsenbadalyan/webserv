#ifndef _READFILE_HPP_
#define _READFILE_HPP_

#include <iostream>
#include <fstream>
#include <unistd.h>

class FileReader
{
private:

	std::string _filename;
	std::string _result;

public:

	FileReader();
	FileReader(const std::string & filename);
	FileReader(const std::string & filename, const std::string & result);
	FileReader(const FileReader & other);

	FileReader & operator=(const FileReader & other);
	FileReader & operator<<(const std::string & resilt);
	FileReader & operator>>(std::string & result);

	~FileReader();

	std::string Read();
	FileReader & Write();
	std::string getResult();
	FileReader & setResult(const std::string & result);
	std::string getFilename();
	FileReader & setFilename(const std::string & filename);
	FileReader & cleanFile();

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