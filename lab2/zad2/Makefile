CC = gcc -Wall -g

compile:
	$(CC) -o main main.c

clean:
	rm -f main

test:
	make clean
	make compile
	./main ../.. -mtime +3 -maxdepth 3
	./main ../.. -mtime 3 -maxdepth 3
	./main ../.. -mtime -3 -maxdepth 3
	./main ../.. -atime +3 -maxdepth 3
	./main ../.. -atime 3 -maxdepth 3
	./main ../.. -atime -3 -maxdepth 3

simple-test:
	make clean
	make compile
	./main ../.. -mtime 2 -maxdepth 5
