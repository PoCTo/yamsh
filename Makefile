default: 
	gcc src/*.c -Wall -g -o yamsh

clean:
	rm -f yamsh.exe
	rm -f yamsh
	rm -rf *.dump
	rm -rf *.stackdump	
