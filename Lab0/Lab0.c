#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char nonPrintable[33][5] = {
		"NUL",
		"SOH",
		"STX",
		"ETX",
		"EOT",
		"ENQ",
		"ACK",
		"BEL",
		"BS",
		"TAB",
		"LF",
		"VT",
		"FF",
		"CR",
		"SO",
		"SI",
		"DLE",
		"DC1",
		"DC2",
		"DC3",
		"DC4",
		"NAK",
		"SYN",
		"ETB",
		"CAN",
		"EM",
		"SUB",
		"ESC",
		"FS",
		"GS",
		"RS",
		"US",
		"Space"
	};

	int firstArg = 1;

	if(argv[1][0] == '-')
		firstArg = 2;

	int readSize = 256; 
	int fd = open(argv[firstArg], O_RDONLY);
	char* buffer = malloc(readSize * sizeof(char));
	
	if (fd != -1)
	{
		read(fd, buffer, readSize);
	}
	else
	{
		int i;
		
		int characterIndex = 0;

		for(i = firstArg; i < argc; i++)
		{
			int j = 0;

			while(argv[i][j] != '\0')
			{
				if(argv[i][j] != '0' && argv[i][j] != '1')
				{
					printf("Invalid arguement(s).\n");
					return 0;
				}

				buffer[characterIndex] = argv[i][j];
				characterIndex++;
				j++;
			}

			buffer[characterIndex] = ' ';
			characterIndex++;
		}
	}

	printf("Original ASCII    Decimal  Parity\n");
	printf("-------- -------- -------- --------\n");

	int charIndex = 0;
	char currentChar;

	int asciiValue = 0;
	int asciiLength = 8;
	int asciiIndex = 0;

	while(1 == 1)
	{
		currentChar = buffer[charIndex];

		if(currentChar != '\0' && currentChar != ' ' && currentChar != EOF && currentChar != '\n')
		{
			if(currentChar == '1' && asciiIndex != 0)
				asciiValue += pow(2, (asciiLength - 1) - asciiIndex);

			
			printf("%c", currentChar);
			asciiIndex++;
		}
		else
		{
			if(currentChar == EOF || currentChar == '\0')
				break;


			if(asciiIndex < asciiLength)
			{
				int i;
				for(i = asciiIndex; i < asciiLength; i++)
				{
					asciiIndex++;
					printf("0");
				}
			}


			if(asciiValue > 32)
			{
				printf("        %c      %d", asciiValue, asciiValue);
			}
			else
			{
				printf("        %s      %d", nonPrintable[asciiValue], asciiValue);
			}

			if(asciiValue % 2 == 0)
				printf("  EVEN\n");
			else
				printf("  ODD\n");

			asciiIndex = 0;
			asciiValue = 0;
		}

		charIndex++;
	}

	return 0;
}
