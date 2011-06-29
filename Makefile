Huffman: main.o Huffman.o
	g++ -o Huffman main.o Huffman.o
main.o: main.cpp Huffman.h
	g++ -c -Wall main.cpp
Huffman.o: Huffman.cpp Huffman.h
	g++ -c -Wall Huffman.cpp
.PHONY: clean
clean:
	rm -f Huffman.o main.o Huffman
