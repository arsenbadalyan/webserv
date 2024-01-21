#define _PARSINGCONFIGFILE_HPP_
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

};

#endif