CC = gcc -Wall -ggdb

all: test

compile:
	make clean
	$(CC) -o main main.c

test: 
		make clean
		make matrix-gen
		$(CC) -o main main.c
		./main lista 2 1000 2
		./matrix test
clean:
	rm -f main matrix
	rm -f -r .tmp

matrix-gen: 
	$(CC) -o matrix matrix.c
	./matrix 10 10