CC=gcc
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835
INCLUDES=-Iinclude
EXECUTABLE=SNESDev

SNESDev: obj/main.o obj/SNESpad.o
	$(CC) obj/main.o obj/SNESpad.o $(LIBS) -o bin/$(EXECUTABLE)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/main.c -o obj/main.o

obj/SNESpad.o: src/SNESpad.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) src/SNESpad.c -o obj/SNESpad.o

clean:
	rm obj/*.o; rm bin/$(EXECUTABLE)

