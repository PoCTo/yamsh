#CC=clang

default: 
	${CC} src/*.c -g -Wall -o build/yamsh
	${CC} src/util/ls.c -g -Wall -o build/util/ls
	${CC} src/util/pwd.c -g -Wall -o build/util/pwd
	${CC} src/util/cp.c -g -Wall -o build/util/cp
	${CC} src/util/mv.c -g -Wall -o build/util/mv
	${CC} src/util/ln.c -g -Wall -o build/util/ln
	${CC} src/util/wc.c -g -Wall -o build/util/wc
	${CC} src/util/cat.c -g -Wall -o build/util/cat
	${CC} src/util/grep.c -g -Wall -o build/util/grep

all: yamsh

clean:
	rm -f yamsh.exe
	rm -f yamsh
	rm -rf *.dump
	rm -rf *.stackdump	
	rm -rf src/*.o
	rm -rf build/yamsh
	rm -rf build/util/*
