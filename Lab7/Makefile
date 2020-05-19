all: matrix_gen matrix_add clean

matrix_gen: matrix_gen.c
	gcc -c matrix_gen.c
	gcc -o matrix_gen matrix_gen.c -Wall -Wextra

matrix_add: matrix_add.c
	gcc -c matrix_add.c
	gcc -o matrix_add matrix_add.c -Wall -Wextra

clean: matrix_gen.o matrix_add.o
	rm matrix_gen.o
	rm matrix_add.o
