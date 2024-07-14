#ifndef _PARSINGCONFIGFILE_HPP_
#define _PARSINGCONFIGFILE_HPP_

#include "Server.hpp"
#include "FileReader.hpp"
#include <dirent.h>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <set>

class ParsingConfigFile {
private:

	// std::string	_filename;
	FileReader			_file;
	std::string			_data;
	std::vector<Server>	_serverList;
	std::set<int>		_statusCode;
	// size_t				_sizeList;

public:
	ParsingConfigFile();
	ParsingConfigFile(const std::string & filename);
	ParsingConfigFile( const ParsingConfigFile & other);
	ParsingConfigFile & operator=(const ParsingConfigFile & other);
	~ParsingConfigFile();

	void				setFile(const std::string & filename);
	std::vector<Server>	startPars();

private:

	bool	CheckSintex();
	bool	CheckCorecktServer();
	size_t	get_reng(size_t start);
	bool	CheckCoreckt(size_t pos_start, size_t pos_end, bool controlFlag);
	size_t	CheckCorecktLocation(size_t pos_start, size_t pos_end);
	size_t	CheckCorecktConfig(std::string config, size_t pos_start, size_t pos_end, bool controlFlag);

	void	CheckAndCreatCorektPath();

	size_t	checkListen(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkServerName(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkRoot(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkAllowMethods(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkUploadDir(size_t pos_start, size_t pos_end, bool controlFlag);

	size_t	checkErrorPage(size_t pos_start, size_t pos_end, bool controlFlag);
	
	size_t statusCodesErrorPage(std::string number);

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

	void	removeComent();
	std::string getErrorline(size_t pos);

	size_t checkGigabyte(std::string &BodySize, size_t pos_start);
	size_t checkMegabyte(std::string &BodySize, size_t pos_start);
	size_t checkKilobyte(std::string &BodySize, size_t pos_start);
	size_t checkByte(std::string &BodySize, size_t pos_start);
	size_t findSimbol(std::string &BodySize, size_t pos_start);

	bool	checkSinv(const std::string &sin);
	void	CleaningTheLocationName(std::string &loc_name);
	bool	checkLocName(const std::string sin);
	void	addErrorPathForLocation();

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