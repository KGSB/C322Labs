#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int cPid = fork();

	if (cPid < 0)
	{
		printf("fork failed");
		return 1;
	}
	else if (cPid == 0) //child
	{
		char* newArgs[argc];
		int i;
		for(i=0; i < argc - 1; i++)
		{
			newArgs[i] = argv[i+1];
		}

		newArgs[argc - 1] = NULL;

		char* envp[] = {(char*)0};
		execve(argv[1], newArgs, envp);

		exit(EXIT_SUCCESS);
	}
	else //parent
	{
		fprintf(stderr, "Child PID: %d\n", cPid);
		
		int childReturnValue;
		int wStatus;

		do
		{
			waitpid(cPid, &wStatus, WUNTRACED);
			childReturnValue = WIFEXITED(wStatus);

		} while(!WIFEXITED(wStatus));

		fprintf(stderr, "Child Return Value: %d\n", childReturnValue);
	}

	return 0;
}
