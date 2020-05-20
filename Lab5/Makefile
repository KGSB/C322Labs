all: PageOffset clean

PageOffset: PageOffset.c
	gcc -c PageOffset.c
	gcc -o PageOffset PageOffset.c -Wall -Wextra

clean: PageOffset.o
	rm PageOffset.o
