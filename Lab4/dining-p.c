#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

enum philosopherState
{
	Active,
	Inactive
};

struct philosopher
{
	int cycleCount;
	enum philosopherState currentState;
	char semaphoreNames[2][12];
	sem_t* semaphores[2];
};

int currentPhilosopher = 0;
int minTime = 500;
int maxTime = 1500;
struct philosopher philosophers[5];

char semaphoreNames[5][12] = {
	"/chopstick1\0",
	"/chopstick2\0",
	"/chopstick3\0",
	"/chopstick4\0",
	"/chopstick5\0",
};

int randomTime()
{
	//converting from micro to milli
	return (rand() % ((maxTime * 1000) - (minTime * 1000) + 1)) + (minTime * 1000);
}

void eat(int i)
{
	printf("Philosopher #%d is eating.\n", i);
	usleep(randomTime());
}

void think(int i)
{
	printf("Philosopher #%d is thinking.\n", i);
	usleep(randomTime());
}

void handler(int signum)
{
	if(signum == SIGTERM)
	{
		philosophers[currentPhilosopher].currentState = Inactive;

		if(philosophers[currentPhilosopher].semaphores[0] != NULL)
		{
			int close = sem_close(philosophers[currentPhilosopher].semaphores[0]);
			printf("Philosopher #%d lock 0 exited with [close: %d]\n", currentPhilosopher, close);
		}
		else
			printf("philosopher %d lacked lock 0.\n", currentPhilosopher);

		if(philosophers[currentPhilosopher].semaphores[1] != NULL)
		{
			int close = sem_close(philosophers[currentPhilosopher].semaphores[1]);
			printf("Philosopher #%d lock 1 exited with [close: %d]\n", currentPhilosopher, close);
		}
		else
			printf("philosopher %d lacked lock 1.\n", currentPhilosopher);

		fprintf(stderr, "Philosopher #%d completed %d cycles.\n", currentPhilosopher, philosophers[currentPhilosopher].cycleCount);
	}
	
	signal(SIGTERM, handler);
}

int main()
{
	int philosopherCount = 5;
	sem_t* chopstick[5];
	
	signal(SIGTERM, handler);
	printf("PID: %d\n", getpid());

	int i;
	for(i = 0; i < philosopherCount; i++)
	{
		philosophers[i].currentState = Active; 
	}

	while(1 == 1)
       	{
		if(philosophers[currentPhilosopher].currentState == Inactive)
		{
			currentPhilosopher++;
			continue;
		}

		//same number of chopsticks as philosophers
		for(i = 0; i < philosopherCount; i++)
		{
			int sVal = 0;
			if(chopstick[i] != NULL)
				sem_getvalue(chopstick[i], &sVal);
			
			if(sVal > 0 || chopstick[i] == NULL)
			{
				printf("opening\n");
				chopstick[i] = sem_open(semaphoreNames[i], O_CREAT, S_IXUSR, 1);
				printf("here\n");
				int j;
				for(j = 0; j < 2; j++)
				{
					if(philosophers[currentPhilosopher].semaphores[j] == NULL)
					{
						philosophers[currentPhilosopher].semaphores[j] = chopstick[i];
						strcpy(philosophers[currentPhilosopher].semaphoreNames[j], semaphoreNames[i]);
						sem_trywait(chopstick[i]);
					}
				}
			}
		}
		
		//if index of 1 isn't null, then the philospher has 2 semaphores since they are assigned in order
		if(philosophers[currentPhilosopher].semaphores[1] != NULL)
		{
			//eat
			eat(currentPhilosopher);
			//sem_post(philosophers[currentPhilosopher].semaphores[0]);
			//sem_post(philosophers[currentPhilosopher].semaphores[1]);
		}
		else
		{
			//think
			think(currentPhilosopher);
		}

		philosophers[currentPhilosopher].cycleCount++;
		//printf("philosopher
		

		int inactiveCount = 0;
		for(i = 0; i < philosopherCount; i++)
		{
			if(philosophers[i].currentState == Inactive)
				inactiveCount++;
		}
		
		if(inactiveCount  == philosopherCount)
			return 0;

		currentPhilosopher++;

		if(currentPhilosopher > philosopherCount)
			currentPhilosopher = 0;

		//sem_wait(chopstick[currentPhilosopher]);
		//sem_wait(chopstick[(currentPhilosopher + 1) % philosopherCount]);
		//eat(currentPhilosopher);
		//signal(SIGTERM, handler);//chopstick[currentPhilosopher]);
		//signal(SIGTERM, handler);//chopstick[(currentPhilosopher + 1) % 5]);
		//think(currentPhilosopher);
		//philosophers[currentPhilosopher].cycleCount++;
		//currentPhilosopher++;
	}
	
	//returnVal = sem_open(sem1, O_CREAT | O_EXCL, 0666, 1);

	//if (returnVal == SEM_FAILED)
	//	returnVal = sem_open(sem1, 0);

	//chopstick[1] = returnVal;
	//sleep(1);
	//sem_close(chopstick[1]);
	//sem_unlink(sem1);

	return 0;
}
