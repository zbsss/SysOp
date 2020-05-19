clean:
	rm -f main Times.txt output.txt

build:
	make clean
	gcc -o main main.c -lpthread -lm -g

test:
	make build
	make test-sign
	make test-block
	make test-inter

test-sign:
	./main 1 sign apollonian_gasket.ascii.pgm output.txt
	./main 2 sign apollonian_gasket.ascii.pgm output.txt
	./main 4 sign apollonian_gasket.ascii.pgm output.txt
	./main 8 sign apollonian_gasket.ascii.pgm output.txt
	
test-block:
	./main 1 block apollonian_gasket.ascii.pgm output.txt
	./main 2 block apollonian_gasket.ascii.pgm output.txt
	./main 4 block apollonian_gasket.ascii.pgm output.txt
	./main 8 block apollonian_gasket.ascii.pgm output.txt

test-inter:
	./main 1 interleaved apollonian_gasket.ascii.pgm output.txt
	./main 2 interleaved apollonian_gasket.ascii.pgm output.txt
	./main 4 interleaved apollonian_gasket.ascii.pgm output.txt
	./main 8 interleaved apollonian_gasket.ascii.pgm output.txt