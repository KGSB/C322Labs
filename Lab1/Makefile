#Name: Prince Cortez
#Project: Lab1 Time and Forking Children

EXECUTABLES=time-4-baby-and-me
SRC=Lab1.c
OBJ=Lab1.o

CC=gcc
CFLAGS=-g

$(EXECUTABLES):$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	#gcc -c Lab1.c -Wall -Wextra
	#gcc -o Lab1 Lab1.c -lm -Wall -Wextra

clean:
	rm -f $(EXECUTABLES) $(OBJ)
