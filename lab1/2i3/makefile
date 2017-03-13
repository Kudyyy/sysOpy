CC=gcc
CFLAGS=-Wall -std=c99
OPT=-O2
OUTPUT=result.txt

all: OPT lstatic lshared ldynamic

OPT: 
	echo $(OPT) >> $(OUTPUT)
	echo "\n" >> $(OUTPUT)

lstatic: main.c
	cp ../1/libstatic.a .
	$(CC) $(CFLAGS) $(OPT) $^ -L. -$@ -o $@.out
	echo "static\n" >> $(OUTPUT)
	./$@.out >> result.txt

lshared: main.c
	cp ../1/libshared.so .
	$(CC) $(CFLAGS) $(OPT) $^ -L. -$@ -o $@.out
	echo "shared\n" >> $(OUTPUT)
	./$@.out >> result.txt

ldynamic: mainDyn.c
	$(CC) $(CFLAGS) $(OPT) $^ -o $@.out -ldl
	echo "dynamic\n" >> $(OUTPUT)
	./$@.out >> result.txt

clean:
	rm -f *.out *.txt *.so *.a
