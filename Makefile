#CC=clang

default: 
	${CC} src/*.c -g -Wall -o build/yamsh
	${CC} src/util/ls.c -g -Wall -o build/util/ls
	${CC} src/util/pwd.c -g -Wall -o build/util/pwd

all: yamsh

yamsh: memory.o list.o str.o tree.o main.o test.o parse.o memory.o
	gcc src/*.o -o yamsh

main.o: src/main.c
	gcc -Wall -o src/main.o src/main.c
  
test.o: src/test.c
	gcc -Wall -o src/test.o src/test.c

parse.o: src/tree.c
	gcc -Wall -o src/parse.o src/parse.c

tree.o: src/tree.c
	gcc -Wall -o src/tree.o src/tree.c

str.o: src/str.c
	gcc -Wall -o src/str.o src/str.c

list.o: src/list.c
	gcc -Wall -o src/list.o src/list.c

memory.o: src/memory.c
	gcc -Wall -o src/memory.o src/memory.c

clean:
	rm -f yamsh.exe
	rm -f yamsh
	rm -rf *.dump
	rm -rf *.stackdump	
	rm -rf src/*.o
	rm -rf build/yamsh
	rm -rf build/util/*
