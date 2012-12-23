CC=gcc
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835 -lrt 
INCLUDES=-Iinclude
EXECUTABLE=testButton

testButton: main.o cpuinfo.o
	$(CC) main.o cpuinfo.o $(LIBS) -o $(EXECUTABLE)

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) main.c -o main.o

cpuinfo.o: cpuinfo.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) cpuinfo.c -o cpuinfo.o

clean:
	rm *.o; rm $(EXECUTABLE)

