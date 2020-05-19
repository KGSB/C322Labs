#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int size = atoi(argv[1]);

	int** blocks = (int**)malloc(size * sizeof(int*));

	int i;
	for(i = 0; i < size; i++)
	{
		blocks[i] = (int*)malloc(size * sizeof(int));
	}

	//stores the matrix data in **blocks
	for(i = 0; i < size; i++)
	{
		//4 is the largest possible size -100
		char buffer[4];
		char currentChar;
		int bufferIndexer = 0;
		int currentNum = 0;

		while (1 == 1)
		{
			currentChar = fgetc(stdin);
			
			if(currentChar == ' ' || currentChar == '\n')
			{
				if(currentNum == size)
					break;

				blocks[i][currentNum] = atoi(buffer);				
				currentNum++;

				int j;
				for(j = 0; j < 4; j++)
					buffer[j] = '\0';

				bufferIndexer = 0;
			}
			else
			{
				buffer[bufferIndexer] = currentChar;
				bufferIndexer++;
			}
		}
	}

	for(i = 0; i < size; i++)
	{
		int j;
		for(j = 0; j < size; j++)
		{
			printf("%d ", blocks[i][j]);
		}
		printf("\n");
	}
	
	//int scalar = (rand() % (max - min + 1) + min);

	//time_t startTime = time(NULL);

	//int size = atoi(argv[1]);
	//int blocks = atoi(argv[2]);


	//time_t elapsedTime = time(NULL) - startTime;
	//fprintf(stderr, "%ld\n", elapsedTime);
}
