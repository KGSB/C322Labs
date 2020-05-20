#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		exit(EXIT_FAILURE);
	}

	//4KB = 4096
	int pageSize = 4096;
	unsigned int address = atoi(argv[1]);
	unsigned int pageNumber = address / pageSize;
	unsigned int offset = address % pageSize;

	printf("Address %d contains:\n", address);
	printf("Page Number = %d\n", pageNumber);
	printf("Offset = %d\n", offset);
	return 0;
}
