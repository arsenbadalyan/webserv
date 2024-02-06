#ifndef _PARSINGCONFIGFILE_HPP_
#define _PARSINGCONFIGFILE_HPP_

#include "Server.hpp"
#include "ReadFile.hpp"
#include <dirent.h>
#include <sstream>

class ParsingConfigFile {
private:

	// std::string	_filename;
	ReadFile			_file;
	std::string			_data;
	std::vector<Server>	_serverList;
	// size_t				_sizeList;

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
	bool	CheckCoreckt(size_t pos_start, size_t pos_end, bool controlFlag);
	size_t	CheckCorecktLocation(size_t pos_start, size_t pos_end);
	size_t	CheckCorecktConfig(std::string config, size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkListen(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkServerName(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkRoot(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkAllowMethods(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkUploadDir(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkErrorPage(size_t pos_start, size_t pos_end, bool controlFlag);
	size_t	statusCodes(std::string number);

	size_t	checkReturn(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkCgi(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkIndex(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkAutoindex(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkClientMaxBodySize(size_t pos_start, size_t pos_end, bool controlFlag);


	size_t	runSpaceTab(size_t pos_start, size_t pos_end);
	bool	checCorectHostAndPort(size_t pos_start, size_t pos_end, bool controlFlag);
	bool	chekAndSaveHost(std::string host);
	bool	chekAndSavePort(std::string port);
	bool	chekHostNumber(std::string number);

	size_t checkGigabyte(std::string &BodySize);
	size_t checkMegabyte(std::string &BodySize);
	size_t checkKilobyte(std::string &BodySize);
	size_t checkByte(std::string &BodySize);
	size_t findSimbol(std::string &BodySize);


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