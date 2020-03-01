#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int pid = fork();

	if (pid < 0)
	{
		printf("fork failed");
		return 1;
	}
	else if (pid == 0) //child
	{
		printf("PPID: %d, PID: %d\n", getppid(), getpid());
		exit(EXIT_SUCCESS);
	}
	else //parent
	{
		printf("START: %ld\n", time(NULL));

		int status;
		waitpid(pid, &status, WIFEXITED(status));
		printf("PPID: %d, PID: %d, CPID: %d, RETVALUE: %d\n", getppid(), getpid(), pid, status);	
		
		struct tms buffer;
		times(&buffer);

		printf("USER: %ld, SYS: %ld\n", buffer.tms_utime, buffer.tms_stime);
		printf("CUSER: %ld, CSYS: %ld\n", buffer.tms_cutime, buffer.tms_cstime);
		printf("STOP: %ld\n", time(NULL));
	}

	return 0;
}
