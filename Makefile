build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer
debug:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -g -o renderer
run:
	./renderer
clean:
	rm -f renderer