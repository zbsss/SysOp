CC = gcc -Wall -g

compile:
	$(CC) -o main main.c

clean:
	rm -f main

run:
	make clean
	make compile
	./main ../.. -mtime +3 -maxdepth 5
