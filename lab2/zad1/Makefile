CC = gcc -Wall -g

compile: 
	$(CC) -o main main.c

clean:
	rm -f main
	rm -f *.txt
test:
	make clean
	make compile
	./main generate records.txt 100 100 
	cat records.txt
	./main sort sys records.txt 100 100
	cat records.txt
