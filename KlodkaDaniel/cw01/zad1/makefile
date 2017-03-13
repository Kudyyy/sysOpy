CC=gcc
CFLAGS=-c -Wall
OBJECTS = contactsL.o contactsT.o person.o

all: libstatic libshared

libstatic: contactsL.c contactsT.c person.c
	$(CC) $(CFLAGS) $^
	ar -cr $@.a $(OBJECTS)

libshared: contactsL.c contactsT.c person.c
	$(CC) $(CFLAGS) -fPIC $^ 
	$(CC) -shared -fPIC -o $@.so $(OBJECTS)


clean:
	rm -f *.a *.o *.so
