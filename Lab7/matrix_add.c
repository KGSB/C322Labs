#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void matrix_add(char* block, int size, int scalar)
{
	//4 is the largest possible size -100
	char buffer[4];
	int bufferIndexer = 0;
	//int currentCharIndex = 0;
	char currentChar;
	int i;
	int currentNum = 0;	
	int* data = malloc(size * sizeof(int));
	int currentReturnIndex = 0;
	//keeps track of the locations of return characters
	int* returnIndices = malloc(size * sizeof(int));
	//-101 is out of bounds for matrix generation
	for(i = 0; i < (size * sizeof(int)); i++)
	{
		data[i] = -101;
		returnIndices[i] = -1;
	}

	//converts blocks to numeric values
	for(i = 0; i < size; i++)
	{
		currentChar = block[i];
			
		if(currentChar == ' ' || currentChar == '\n')
		{
			//there is a return character after currentNum
			if (currentChar == '\n')
			{
				returnIndices[currentReturnIndex] = currentNum;
				currentReturnIndex++;
			}
		//	if(currentNum == size)
		//		break;
			//matrix[i][currentNum] = atoi(buffer);
			data[currentNum] = atoi(buffer);
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

			if(i == size - 1)
			{
				data[currentNum] = atoi(buffer);
				currentNum++;
			}
		}
	}

	int* cutData = malloc((currentNum + 1) * sizeof(int));
	printf("Currentnum: %d\n", currentNum);
	//adds the scalar
	for(i = 0; i < currentNum; i++)
	{
		cutData[i] = data[i] + scalar;
		printf("%d = %d + %d\n", cutData[i], data[i], scalar);
	}
	printf("scalar addition complete.\n");
	currentReturnIndex = 0;
	//converts back to char*
	for(i = 0; i < currentNum; i++)
	{
		printf("to be added: %d\n", cutData[i]);
		sprintf(buffer, "%d", cutData[i]);
		printf("Buffer: %s\n", buffer);
		strcat(block, buffer);
		strcat(block, " ");

		if(returnIndices[currentReturnIndex] == i)
		{
			strcat(block, "\n");
			currentReturnIndex++;
		}
	}
	printf("\nstuff: %s\n", block);
}

int main(int argc, char** argv)
{
	time_t startTime = time(NULL);
	
	//using bounds from the generator
	int min = -100;
	int max = 100;
	int scalar = (rand() % (max - min + 1) + min);
	int size = atoi(argv[1]);
	int blocks = atoi(argv[2]);
	int blockSize = size / blocks;

	
	int** matrix = (int**)malloc(size * sizeof(int*));

	int i;
	for(i = 0; i < size; i++)
	{
		matrix[i] = (int*)malloc(size * sizeof(int));
	}

	//stores the matrix data
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

				matrix[i][currentNum] = atoi(buffer);				
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

	/*prints matrix data
	for(i = 0; i < size; i++)
	{
		int j;
		for(j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}*/

	//we are assuming the format specified in the assignment writeup is being used
	char fileName[20] = "matrix_file.";
	char holder[7];
	sprintf(holder, "%d", size);
	strcat(fileName, holder);
	fileName[19] = '\0';
	
	int fd = open(fileName, O_RDONLY);
	char* crap = malloc(sizeof(char) * 8);
	//read(fd, buf, 3);
	
	struct aiocb ioInfo;
	ioInfo.aio_fildes = fd;
	ioInfo.aio_buf = crap;
	ioInfo.aio_nbytes = 8;
	ioInfo.aio_offset = 0;
	ioInfo.aio_sigevent.sigev_notify = SIGEV_NONE;
	
	int readStatus = aio_read(&ioInfo);
	while (aio_return(&ioInfo) != 8)
		printf("Return: %ld\n", aio_return(&ioInfo));
	matrix_add(crap, 8, 1);
	printf("Post Add: %s\n", crap);

	//int i;
	for(i = 0; i < blocks; i++)
	{
		int j;
		for(j = 0; j < blocks; j++)
		{
			
		}
	}
	
	close(fd);
	time_t elapsedTime = time(NULL) - startTime;
	fprintf(stderr, "Time: %ld\n", elapsedTime);
}
