#ifndef _RADFILE_HPP_
#define _READFILE_HPP_

#include <string>

class ReadFile{
private:

	std::string _filename;
	std::string _result;

public:

	ReadFile(std::string filename);
	ReadFile(const ReadFile & other);
	ReadFile & operator=(const ReadFile & other);
	~ReadFile();

	bool start();
	std::string Get();

}

#endif