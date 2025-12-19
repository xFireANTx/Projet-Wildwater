all: c-wildwater

c-wildwater: codeC/main.o codeC/lecture.o codeC/avl.o codeC/structure.o
	gcc -o c-wildwater codeC/main.o codeC/lecture.o codeC/avl.o codeC/structure.o -lm

codeC/main.o: codeC/main.c codeC/lecture.h codeC/avl.h codeC/structure.h
	gcc -c codeC/main.c -o codeC/main.o -Wall -Wextra -std=c11

codeC/lecture.o: codeC/lecture.c codeC/lecture.h codeC/structure.h
	gcc -c codeC/lecture.c -o codeC/lecture.o -Wall -Wextra -std=c11

codeC/avl.o: codeC/avl.c codeC/avl.h
	gcc -c codeC/avl.c -o codeC/avl.o -Wall -Wextra -std=c11

codeC/structure.o: codeC/structure.c codeC/structure.h
	gcc -c codeC/structure.c -o codeC/structure.o -Wall -Wextra -std=c11

clean:
	rm -f c-wildwater
	rm -f codeC/*.o
