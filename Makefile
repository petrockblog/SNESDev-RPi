CC=gcc
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835 -lrt
INCLUDES=-Iinclude
EXECUTABLE=SNESDev

SNESDev: obj/main.o obj/SNESpad.o
	$(CC) obj/main.o obj/SNESpad.o obj/cpuinfo.o $(LIBS) -o bin/$(EXECUTABLE)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/main.c -o obj/main.o

obj/SNESpad.o: src/SNESpad.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/SNESpad.c -o obj/SNESpad.o

obj/cpuinfo.o: src/cpuinfo.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/cpuinfo.c -o obj/cpuinfo.o

clean:
	rm obj/*.o; rm bin/$(EXECUTABLE)

