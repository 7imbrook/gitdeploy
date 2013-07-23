all: build

build: 
	gcc src/main.c -L /usr/local/lib/ -lgit2 -o dgit

clean:
	rm dgit