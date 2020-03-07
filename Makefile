CC = gcc -Wall

static:
	make clean
	$(CC) -g -c lib.c
	ar rcs lib.a lib.o
	$(CC) -g -c main.c
	$(CC) -g main.o lib.a -o main

shared:
	make clean
	$(CC) -c -fPIC lib.c
	$(CC) -shared -fPIC -o liblib.so lib.o
	$(CC) main.c -o main -L. 

clean:
	rm -f *.o 
	rm -f *.a
	rm -f *.so
	rm -f tempFile
	rm -f main