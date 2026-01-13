all: app

app: build/cipher.o
	cc build/cipher.o -Llib -lceasar -lrot13 -lvigenere -o app

build/cipher.o: src/cipher.c
	mkdir -p build
	cc -Ilibinc -c src/cipher.c -o build/cipher.o

clean:
	rm -f build/cipher.o app

