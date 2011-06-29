#include "Huffman.h"
#include <iostream>
#include <cstring>

int main(int argc, char *argv[])
{
	HuffmanTree h;
	
	if(argc != 4)
	{
		PrintUsage(argv[0]);
		exit(1);
	}
	
	if(!strcmp(argv[1],"-c")||!strcmp(argv[1],"-C"))
		h.Compress(argv[2],argv[3]);
	else if(!strcmp(argv[1],"-d")||!strcmp(argv[1],"-D"))
		h.Decompress(argv[2],argv[3]);
	else{
		PrintUsage(argv[0]);
		exit(1);
	}

    return 0;
}

