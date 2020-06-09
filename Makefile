all: test_graphe

test_graphe: test_graphe.o graphe.o io_graphe.o pile.o file.o
	gcc -o test_graphe test_graphe.o graphe.o io_graphe.o

graphe.o: graphe.c graphe.h pile.h file.h
	gcc -Wall -c graphe.c

io_graphe.o: io_graphe.c graphe.h
	gcc -Wall -c io_graphe.c

test_graphe.o: test_graphe.c graphe.h
	gcc -Wall -c test_graphe.c

pile.o : pile.c pile.h
	gcc -Wall -c pile.c


file.o : file.c file.h
	gcc -Wall -c file.c

clean:
	rm -f *.o test_graphe  *~ data/*~
