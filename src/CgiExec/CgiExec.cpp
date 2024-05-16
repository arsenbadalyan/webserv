#include "CgiExec.hpp"

std::string CgiExec::executeCGI(std::string filePath, std::string *fileType)
{

	(void)fileType;

	int pipefd[2];
	char buffer[2];
	std::string res;
	ssize_t bytesRead;

	char *args[] = {strdup(PYTHONPATH), strdup(filePath.c_str()), NULL,NULL};

	if (pipe(pipefd) == -1) 
	    throw std::runtime_error("pipe");

	int pid = fork();

	if (pid == -1) 
	    throw std::runtime_error("fork");
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			throw std::runtime_error("dub2");
	    close(pipefd[1]);
	    if (execve(PYTHONPATH, args, NULL) == -1)//env or NULL
	        throw std::runtime_error("execve");
	}
	waitpid(pid, NULL, 0);
	close(pipefd[1]);
	while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
	{
	    buffer[bytesRead] = '\0'; 
	    res += buffer;
	}
	close(pipefd[0]);

	free(args[0]);
	free(args[1]);
	return res;
}