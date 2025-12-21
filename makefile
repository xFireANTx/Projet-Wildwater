all: exe

exe: main.o avl.o basic_functions.o avl_histo_reel.o avl_histo_traitement.o
	gcc -o exe main.o avl.o basic_functions.o avl_histo_reel.o avl_histo_traitement.o

main.o: main.c basic_functions.h avl.h avl_histo_reel.h avl_histo_traitement.h
	gcc -c main.c -o main.o 

avl.o: avl.c avl.h basic_functions.h
	gcc -c avl.c -o avl.o 

basic_functions.o: basic_functions.c basic_functions.h avl.h
	gcc -c basic_functions.c -o basic_functions.o 

avl_histo_reel.o: avl_histo_reel.c avl_histo_reel.h
	gcc -c avl_histo_reel.c -o avl_histo_reel.o 

avl_histo_traitement.o: avl_histo_traitement.c avl_histo_traitement.h
	gcc -c avl_histo_traitement.c -o avl_histo_traitement.o 

clean:
	rm -f exe
	rm -f *.o
