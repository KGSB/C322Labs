#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct argInformation
{
	char*** separatedArgs;
	int commandCount;
	int* argCounts;
};

struct argInformation separateArgs(int argc, char** argv)
{
	struct argInformation returnInformation;

	int commandCount = 1;
	int currentArg;
	for(currentArg = 0; currentArg < argc; currentArg++)
	{
		if(argv[currentArg][0] == ',')
		{
			commandCount++;
		}
	}

	returnInformation.commandCount = commandCount;
	returnInformation.argCounts = malloc(sizeof(int) * commandCount);
	char*** separatedArgs = malloc(sizeof(char**) * commandCount);

	//don't want tube to be included in passed arguments
	int prevLastArgIndex = 1;
	int currentCommand;
	for(currentCommand = 0; currentCommand < commandCount; currentCommand++)
	{
		int startIndex = prevLastArgIndex;
		//each set of args must have a null end
		int argCount = 1;

		if(currentCommand == 0)
		{
			argCount = 1;
		}
		
		for(currentArg = prevLastArgIndex; currentArg < argc; currentArg++)
		{
			if(argv[currentArg][0] == ',')
			{
				prevLastArgIndex = currentArg + 1;
				break;
			}
			else if(currentArg == argc - 1)
			{
				prevLastArgIndex = argc;
			}

			argCount++;
		}

		returnInformation.argCounts[currentCommand] = argCount;
		separatedArgs[currentCommand] = malloc(sizeof(char*) * argCount);
		
		int currentSeparatedArg = 0;
		for(currentArg = startIndex; currentArg < prevLastArgIndex; currentArg++)
		{
			if(argv[currentArg][0] == ',')
			{
				continue;
			}

			separatedArgs[currentCommand][currentSeparatedArg] = argv[currentArg];
			currentSeparatedArg++;
		}
	}
	returnInformation.separatedArgs = separatedArgs;
	return returnInformation;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		return 0;
	}

	//0 read, 1 write
	//pipeFd1 - write from child1 to child2
	//pipeFd1 - write from child2 to child1
	int pipeFd1[2];
	int pipeFd2[2];
	int child1;
	int child2;
	char* envp[] = {(char*)0};
	struct argInformation separatedArgs = separateArgs(argc, argv);
	
	int EXECUTION_NOT_STARTED = 0;
	int EXECUTION_STARTED = 1;
	int EXECUTION_COMPLETE = 2;
	int executionStatus[separatedArgs.commandCount];

	int i;
	for(i = 0; i < separatedArgs.commandCount; i++)
	{
		//child1 takes i=0 and child2 takes i=1 by default
		if(i == 0 || i ==1)
		{
			executionStatus[i] = EXECUTION_STARTED;
		}
		else
		{
			executionStatus[i] = EXECUTION_NOT_STARTED;
		}
	}
	if(pipe(pipeFd1) == -1 || pipe(pipeFd2) == -1)
	{
		printf("pipe couldn't be created.\n");
		exit(EXIT_FAILURE);
	}
	
	child1 = fork();
	
	//only want this process making a child
	//don't want child 1 to create a child
	if(child1 > 0)
	{
		child2 = fork();
	}

	if (child1 < 0 || child2 < 0)
	{
		exit(EXIT_FAILURE);
	}
	else if (child1 == 0)
	{
		//child1
		int readFd = pipeFd2[0];
		int writeFd = pipeFd1[1];
		close(pipeFd1[0]);
		close(pipeFd2[1]);
		fcntl(readFd, F_SETFL, O_NONBLOCK);
		
		//0 - index, 1 - status
		int pipeBufferSize = 3;
		char pipeBuffer[pipeBufferSize];
		
		int jobIndex = 0;
		int readStatus;
		int workToBeDone = 1;
		execve(separatedArgs.separatedArgs[jobIndex][0], separatedArgs.separatedArgs[jobIndex], envp);
		executionStatus[jobIndex] = EXECUTION_COMPLETE;
		pipeBuffer[0] = jobIndex;
		pipeBuffer[1] = executionStatus[jobIndex];
		pipeBuffer[2] = '\0';
		write(writeFd, pipeBuffer, pipeBufferSize);

		while(workToBeDone == 1)
		{
			//loop should start here
			while (1 == 1)
			{
				readStatus = read(readFd, pipeBuffer, pipeBufferSize + 1);
				printf("Child1 Read Status: %d\n", readStatus);
				//buffer empty
				if (readStatus == 0)
				{
					break;
				}

				executionStatus[(int)pipeBuffer[0]] = (int)pipeBuffer[1];
			}

			for(i = 0; i < separatedArgs.commandCount; i++)
			{
				if(executionStatus[i] == EXECUTION_NOT_STARTED)
				{
					printf("Child1 taking job %d\n", jobIndex);
					jobIndex = i;
					break;
				}

				if(i == separatedArgs.commandCount)
				{
					//no more jobs to run
					workToBeDone = 0;
				}
			}
		

			if(workToBeDone == 0)
			{
				break;
			}

			executionStatus[jobIndex] == EXECUTION_STARTED;
			pipeBuffer[0] = jobIndex;
			pipeBuffer[1] = executionStatus[jobIndex];
			pipeBuffer[2] = '\0';
			write(writeFd, pipeBuffer, pipeBufferSize);
			execve(separatedArgs.separatedArgs[jobIndex][0],
					separatedArgs.separatedArgs[jobIndex], envp);
			
			//alter this buffer to show it's complete
			executionStatus[jobIndex] == EXECUTION_COMPLETE;
			//alter data in buffer to send to reflect this change
			pipeBuffer[1] = executionStatus[jobIndex];
			write(writeFd, pipeBuffer, pipeBufferSize);
			
			//get status update from child2
			//select job and send status to child2
			//execute job
			//upon completion send status update to child2
		}

		close(readFd);
		close(writeFd);
		exit(EXIT_SUCCESS);
	}
	else if (child2 == 0)
	{
		//child2
		int readFd = pipeFd1[0];
		int writeFd = pipeFd2[1];
		close(pipeFd1[1]);
		//close(pipeFd2[0]);
		fcntl(readFd, F_SETFL, O_NONBLOCK);
				
		//0 - index, 1 - status
		int pipeBufferSize = 3;
		char pipeBuffer[pipeBufferSize];

		int jobIndex = 1;
		int readStatus;
		
		execve(separatedArgs.separatedArgs[jobIndex][0], separatedArgs.separatedArgs[jobIndex], envp);
		executionStatus[jobIndex] = EXECUTION_COMPLETE;
		pipeBuffer[0] = jobIndex;
		pipeBuffer[1] = executionStatus[jobIndex];
		pipeBuffer[2] = '\0';
		write(writeFd, pipeBuffer, pipeBufferSize);
		
		int workToBeDone = 1;

		while(workToBeDone == 1)
		{
			//loop should start here
			while (1 == 1)
			{
				readStatus = read(readFd, pipeBuffer, pipeBufferSize + 1);
				printf("Child1 Read Status: %d\n", readStatus);
				//buffer empty
				if (readStatus == 0)
				{
					break;
				}

				executionStatus[(int)pipeBuffer[0]] = (int)pipeBuffer[1];
			}
			
			for(i = 0; i < separatedArgs.commandCount; i++)
			{
				if(executionStatus[i] == EXECUTION_NOT_STARTED)
				{
					printf("Child2 taking job %d\n", i);
					jobIndex = i;
					break;
				}

				if(i == separatedArgs.commandCount)
				{
					//no more jobs to run
					workToBeDone = 0;
				}
			}
		

			if(workToBeDone == 0)
			{
				break;
			}

			executionStatus[jobIndex] == EXECUTION_STARTED;
			pipeBuffer[0] = jobIndex;
			pipeBuffer[1] = executionStatus[jobIndex];
			pipeBuffer[2] = '\0';
			int writeStatus = write(writeFd, pipeBuffer, pipeBufferSize);
			printf("Executing command %d\n", jobIndex);
			execve(separatedArgs.separatedArgs[jobIndex][0],
				separatedArgs.separatedArgs[jobIndex], envp);
		
			//alter this buffer to show it's complete
			executionStatus[jobIndex] == EXECUTION_COMPLETE;
			//alter data in buffer to send to reflect this change
			pipeBuffer[1] = executionStatus[jobIndex];
			write(writeFd, pipeBuffer, pipeBufferSize);
			
			//get status update from child2
			//select job and send status to child2
			//execute job
			//upon completion send status update to child2
		}
		
		close(readFd);
		close(writeFd);
		close(pipeFd2[0]);
		exit(EXIT_SUCCESS);
	}
	else if (child1 > 0 && child2 > 0)
	{
		//parent
		close(pipeFd1[0]);
		close(pipeFd1[1]);
		close(pipeFd2[0]);
		close(pipeFd2[1]);

		fprintf(stderr, "Child1 PID: %d\n", child1);
		fprintf(stderr, "Child2 PID: %d\n", child2);
	
		int child1ReturnValue;
		int child1WStatus;
		int child2ReturnValue;
		int child2WStatus;

		do
		{
			waitpid(child1, &child1WStatus, WUNTRACED);
			waitpid(child2, &child2WStatus, WUNTRACED);

			if(WIFEXITED(child1WStatus))
			{
			child1ReturnValue = WIFEXITED(child1WStatus);
			}
			else
			{
				printf("Something went wrong in child1\n");
			}

			if(WIFEXITED(child2WStatus))
			{
				child2ReturnValue = WIFEXITED(child2WStatus);
			}
			else
			{
				printf("Something went wrong in child2\n");
			}

		} while(!WIFEXITED(child1WStatus) && !WIFEXITED(child2WStatus));

		child1ReturnValue = WIFEXITED(child1WStatus);
		child2ReturnValue = WIFEXITED(child2WStatus);

		printf("Child1 Return Value: %d\n", child1ReturnValue);
		printf("Child2 Return Value: %d\n", child2ReturnValue);
	}

	return 0;
}
