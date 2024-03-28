all: 
	gcc src/main.c -o program -lm
	./program

clear:
	rm program