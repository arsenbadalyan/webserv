#include "ParsingConfigFile.hpp"

int main() {
	
	ParsingConfigFile  a("file");
	try
	{
		a.startPars();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
    return (0);
}