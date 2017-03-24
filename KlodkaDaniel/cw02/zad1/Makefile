CFLAGS= -Wall -std=c99
CC=gcc

compile: 
	$(CC) $(CFLAGS) main.c 

tests: compile
	echo "\n\nTest with size 4 and number of records 1000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 4 1000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 4 1000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 4 1000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 4 1000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 4 1000 >> wyniki.txt

	echo "\n\nTest with size 4 and number of records 2000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 4 2000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 4 2000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 4 2000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 4 2000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 4 2000 >> wyniki.txt

	echo "\n\nTest with size 512 and number of records 1000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 512 1000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 512 1000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 512 1000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 512 1000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 512 1000 >> wyniki.txt

	echo "\n\nTest with size 512 and number of records 2000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 512 2000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 512 2000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 512 2000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 512 2000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 512 2000 >> wyniki.txt

	echo "\n\nTest with size 4096 and number of records 1000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 4096 1000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 4096 1000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 4096 1000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 4096 1000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 4096 1000 >> wyniki.txt

	echo "\n\nTest with size 4096 and number of records 2000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 4096 2000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 4096 2000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 4096 2000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 4096 2000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 4096 2000 >> wyniki.txt

	echo "\n\nTest with size 8192 and number of records 1000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 8192 1000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 8192 1000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 8192 1000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 8192 1000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 8192 1000 >> wyniki.txt

	echo "\n\nTest with size 8192 and number of records 2000\n\n\n" >> wyniki.txt 
	echo "Generate data\n" >> wyniki.txt
	./a.out lib generate ./random_data.txt 8192 2000 >> wyniki.txt
	cp ./random_data.txt ./random_data_copy.txt
	echo "Sorting test\n\n" >> wyniki.txt
	echo "sysSort\n" >> wyniki.txt
	./a.out sys sort ./random_data.txt 8192 2000 >> wyniki.txt
	echo "libSort\n" >> wyniki.txt
	./a.out lib sort ./random_data_copy.txt 8192 2000 >> wyniki.txt
	echo "\nShuffle test\n\n" >> wyniki.txt
	echo "sysShuffle\n" >> wyniki.txt
	./a.out sys shuffle ./random_data.txt 8192 2000 >> wyniki.txt
	echo "libShuffle\n" >> wyniki.txt
	./a.out lib shuffle ./random_data_copy.txt 8192 2000 >> wyniki.txt

clean:
	rm -f *.o *.a *.txt *.out
	rm -f main

