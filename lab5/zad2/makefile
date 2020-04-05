CC = gcc -Wall -std=c99

test:
	make clean
	$(CC) -o producer producer.c
	$(CC) -o consumer consumer.c
	$(CC) -o main main.c
	./main

produce:
	./producer fifo resource_mine/a 5

consume:
	./consumer fifo consumed_resources/output 5

clean:
	rm -f -r fifo producer main consumer