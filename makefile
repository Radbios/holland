all: 
	gcc src/main.c -o program
	./program

clear:
	rm program