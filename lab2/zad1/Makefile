CC = gcc -Wall

compile: 
	$(CC) -o main main.c

clean:
	rm -f main
	rm -f *.txt
test:
	make clean
	make compile
	./main generate records.txt 200 2000