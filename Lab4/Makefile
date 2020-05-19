#Prince Cortez
#Lab4: Rice, rice, and rice

all: dining-p clean

dining-p: dining-p.c
	gcc -c dining-p.c
	gcc -o dining-p dining-p.c -lm -Wall -Wextra -lpthread

clean: dining-p.o
	rm dining-p.o
