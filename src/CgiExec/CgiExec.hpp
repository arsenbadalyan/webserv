#ifndef _CGI_EXEC_
#define _CGI_EXEC_

#include <iostream>
#include <string>
#include <unistd.h>


class CgiExec
{
private:
	static void cp(char* cp, std::string a)
	{
		size_t i = 0;
		for (; i < a.size(); i++)
		{
			cp[i] = a[i];
		}
		cp[i] = 0;
	}

public:
	static int executeCGI(std::string file, std::map<std::string, std::string> &ma)
	{
		int i = file.size() - 1;
		for (; i >= 0; --i)
		{
			if (file[i] == '.')
				break;
		}
		if (i <= 0 || i == static_cast<int>(file.size() - 1))
			return 2;
		std::string filind = std::string(file.begin() + i, file.end());
		std::map<std::string, std::string>::iterator it = ma.find(filind);
		if (it == ma.end())
			return (3);
		int fd[2], w, status;
		std::string res;
		if (pipe(fd) < 0)
			return (1);
		int pid = fork();
		if (pid == 0)
		{
			
			char pythonPath[ma[filind].size() + 1];
			cp(pythonPath, ma[filind]); 
    		char scriptPath[file.size() + 1];
			cp(scriptPath, file);

    		char *args[] = {pythonPath , scriptPath, NULL};
    		char *env[] = {NULL};
			close(fd[0]);
			dup2(fd[1], 1);
    		execve(args[0], args, env);

    		perror("execve");
		}else{
			close(fd[1]);
			// w = waitpid(pid, &status, WUNTRACED);
			w = waitpid(pid, &status, WUNTRACED);
			if (w == -1)
				return (1);
			int fl = 1;
			char a;
			while(fl > 0)
			{
				fl = read(fd[0], &a, 1);
				res += std::string(&a);
			}
			close(fd[0]);
			std::cout << "status == " << status <<std::endl;
			std::cout << "res == \n" << res;

		}
		return (0);
	}
};

#endif