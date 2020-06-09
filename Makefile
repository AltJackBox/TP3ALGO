all: test_graphe
OPTIONS_COMPIL = -Werror -Wall -g

test_graphe: test_graphe.o graphe.o io_graphe.o pile.o file.o
	gcc -o $@ $^

graphe.o: graphe.c graphe.h pile.h file.h
	gcc $(OPTIONS_COMPIL) -c $<

io_graphe.o: io_graphe.c graphe.h
	gcc $(OPTIONS_COMPIL) -c $<

test_graphe.o: test_graphe.c graphe.h
	gcc $(OPTIONS_COMPIL) -c $<

pile.o : pile.c pile.h
	gcc $(OPTIONS_COMPIL) -c $<

file.o : file.c file.h
	gcc $(OPTIONS_COMPIL) -c $<

clean:
	rm -f *.o test_graphe  *~ data/*~
