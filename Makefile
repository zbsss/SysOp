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
	$(CC) -shared -fPIC -o lib.so lib.o
	$(CC) -o main main.c -L. lib.so -I . -Wl,-rpath=`pwd`

dll:
	make clean
	$(CC) -fPIC -c lib.c -o lib.o
	$(CC) -Wl,-rpath=. -fPIC -shared -o lib.so lib.o
	$(CC) -fPIC -L. -o main main_dynamic.c -ldl -D DYNAMIC

test:
	make clean-files
	python3 files/file_generator.py -num 200 -mw 10000
	./main compare_pairs `cat names.txt` remove_operation 0 0 remove_block 0
	#remove_operation 0 0
	#remove_block 0

clean:
	rm -f *.o 
	rm -f *.a
	rm -f *.so
	rm -f tempFile
	rm -f main

clean-files:
	rm -f files/*.txt names.txt
