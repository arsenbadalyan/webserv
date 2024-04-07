#ifndef _CGI_EXEC_
#define _CGI_EXEC_

#include <iostream>
#include <string>
#include <unistd.h>


class CgiExec
{
public:
	static void executeCGI(std::string filePath, std::string fileType)
	{
		int pid = fork();
		if (pid == 0)
		{
			
			const char *pythonPath = "/usr/bin/python3";  
    		const char *scriptPath = "example.py";      

    		char *const args[] = {pythonPath, scriptPath, NULL};
    		char *const env[] = {NULL};

    		execve(pythonPath, args, env);

    		// Код после execve выполнится только в случае ошибки
    		perror("execve");
		}
	}
};

#endif