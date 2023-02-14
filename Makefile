all: room.o building.o hunter.o evidence.o ghost.o main.o
	gcc -g -Wall -pthread -lpthread -o main main.o room.o building.o hunter.o evidence.o ghost.o

room.o:	room.c defs.h
	gcc -c room.c

building.o: building.c defs.h
	gcc -c building.c
	
hunter.o: hunter.c defs.h
	gcc -c hunter.c

evidence.o: evidence.c defs.h
	gcc -c evidence.c

ghost.o: ghost.c defs.h
	gcc -c ghost.c

main.o: main.c defs.h
	gcc -c main.c

clean:
	rm main *.o
