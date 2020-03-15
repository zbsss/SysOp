CC = gcc -Wall -g

compile: 
	$(CC) -o main main.c

clean:
	rm -f main
	make clean-files

clean-files:
	rm -f data.txt data2.txt sdata.txt sdata2.txt ldata.txt ldata2.txt

test:
	make clean
	make compile
	./main generate records.txt 10 5

test-1:
	make clean
	make compile
	./main generate data.txt 10000 1 copy sys data.txt sdata.txt 10000 1 sort sys sdata.txt 10000 1 copy lib data.txt ldata.txt 10000 1 sort lib ldata.txt 10000 1
	./main generate data2.txt 15000 1 copy sys data2.txt sdata2.txt 15000 1 sort sys sdata2.txt 15000 1 copy lib data2.txt ldata2.txt 15000 1 sort lib ldata2.txt 15000 1
	make clean-files

test-4:
	make clean
	make compile
	./main generate data.txt 10000 4 copy sys data.txt sdata.txt 10000 4 sort sys sdata.txt 10000 4 copy lib data.txt ldata.txt 10000 4 sort lib ldata.txt 10000 4
	./main generate data2.txt 15000 4 copy sys data2.txt sdata2.txt 15000 4 sort sys sdata2.txt 15000 4 copy lib data2.txt ldata2.txt 15000 4 sort lib ldata2.txt 15000 4
	make clean-files

test-512:
	make clean
	make compile
	./main generate data.txt 2000 512 copy sys data.txt sdata.txt 2000 512 sort sys sdata.txt 2000 512 copy lib data.txt ldata.txt 2000 512 sort lib ldata.txt 2000 512
	./main generate data2.txt 4000 512 copy sys data2.txt sdata2.txt 4000 512 sort sys sdata2.txt 4000 512 copy lib data2.txt ldata2.txt 4000 512 sort lib ldata2.txt 4000 512
	make clean-files

test-1024:
	make clean
	make compile
	./main generate data.txt 2000 1024 copy sys data.txt sdata.txt 2000 1024 sort sys sdata.txt 2000 1024 copy lib data.txt ldata.txt 2000 1024 sort lib ldata.txt 2000 1024
	./main generate data2.txt 4000 1024 copy sys data2.txt sdata2.txt 4000 1024 sort sys sdata2.txt 4000 1024 copy lib data2.txt ldata2.txt 4000 1024 sort lib ldata2.txt 4000 1024
	make clean-files

test-4096:
	make clean
	make compile
	./main generate data.txt 2000 4096 copy sys data.txt sdata.txt 2000 4096 sort sys sdata.txt 2000 4096 copy lib data.txt ldata.txt 2000 4096 sort lib ldata.txt 2000 4096
	./main generate data2.txt 4000 4096 copy sys data2.txt sdata2.txt 4000 4096 sort sys sdata2.txt 4000 4096 copy lib data2.txt ldata2.txt 4000 4096 sort lib ldata2.txt 4000 4096
	make clean-files

test-8192:
	make clean
	make compile
	./main generate data.txt 2000 8192 copy sys data.txt sdata.txt 2000 8192 sort sys sdata.txt 2000 8192 copy lib data.txt ldata.txt 2000 8192 sort lib ldata.txt 2000 8192
	./main generate data2.txt 4000 8192 copy sys data2.txt sdata2.txt 4000 8192 sort sys sdata2.txt 4000 8192 copy lib data2.txt ldata2.txt 4000 8192 sort lib ldata2.txt 4000 8192
	make clean-files