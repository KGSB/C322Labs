#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int min = -100;
	int max = 100;
	int size = atoi(argv[1]);

	int i;
	for(i = 0; i < size; i++)
	{
		int j;
		for(j=0; j<size; j++)
		{
			int num = (rand() % (max - min + 1) + min);
			printf("%d ", num);
		}

		printf("\n");
	}
}
