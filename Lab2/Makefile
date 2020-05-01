#Name: Prince Cortez
#Project: Lab2 Launch-Tube

all:	launch tube clean
launch: Launch.c
	gcc -c Launch.c
	gcc -o launch Launch.c -lm -Wall -Wextra
	#rm Launch.o
	
tube:	Tube.c
	gcc -c Tube.c
	gcc -o tube Tube.c -lm -Wall -Wextra
	#rm Tube.o

clean: Tube.o Launch.o
	rm Tube.o
	rm Launch.o
