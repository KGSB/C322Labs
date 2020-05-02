#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static int termcount = 0;
static int sigcount = 0;

void handler(int signum)
{
	sigcount++;
	fprintf(stdout, "%s caught at %ld\n", strsignal(signum), time(NULL));
	if(signum == SIGTERM)
	{
		termcount++;
	}
	else
	{
		termcount = 0;
	}

	signal(signum, handler);
}

int main(int argc, char** argv)
{
	int signalcount = 31;
	char signals[31][10] = {
		"NOTSIGNAL\0",
		"SIGHUP\0",
		"SIGINT\0",
		"SIGQUIT\0",
		"SIGILL\0",
		"SIGTRAP\0",
		"SIGIOT\0",
		"SIGBUS\0",
		"SIGFPE\0",
		"SIGKILL\0",
		"SIGUSR1\0",
		"SIGSEGV\0",
		"SIGUSR2\0",
		"SIGPIPE\0",
		"SIGALRM\0",
		"SIGTERM\0",
		"SIGSTKFLT\0",
		"SIGCHLD\0",
		"SIGCONTv\0",
		"SIGSTOP\0",
		"SIGSTOP\0",
		"SIGTTIN\0",
		"SIGTTOU\0",
		"SIGURG\0",
		"SIGXCPU\0",
		"SIGXGSZ\0",
		"SIGVTALRM\0",
		"SIGPROF\0",
		"SIGWINCH\0",
		"SIGIO\0",
		"SIGPWER\0"
	};

	fprintf(stderr, "Catcher PID: %d\n", getpid());

	//disables all signals, we set the ones we care about to a custom handler
	int i;
	for(i = 1; i < signalcount; i++)
	{
		signal(i, SIG_IGN);
	}

	//first arg is the process name
	for(i = 1; i < argc; i++)
	{
		int j;
		//signals[0] just exists to keep the indices matching with actual signal values
		for(j = 1; j < signalcount; j++)
		{
			if (strstr(signals[j], argv[i]) != NULL)
			{
				signal(j, handler);
			}
		}
	}

	while(termcount < 3)
	{
		pause();
	}

	fprintf(stderr, "%d signals caught.\n", sigcount);
	return 0;
}
