test:
	make compile
	./main arguments

compile:
	make clean
	gcc -Wall -g main.c -o main
	gcc -Wall -g child.c -o child

clean:
	rm -f -r child
	rm -f -r main
