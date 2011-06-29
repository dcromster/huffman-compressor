#ifndef HUFFMAN_H
#define HUFFMAN_H

//Huffman.h
//declaration of HuffmanNode class and HuffmanTree class.

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <climits>

//-----------------------------------------------------------------
class HuffmanNode
{
    public:
        int ascii;
        int index;		//decide whether it's the left child
        //or the right one
        int weight;		//weight of the leaf node
        std::string code;	//keep the code of the leaf node
        int parent;		
        int left,right;		//left child and right child
        HuffmanNode():ascii(0),index (0),weight(0),code("0"),parent(0),left(0),right(0){}
        ~HuffmanNode(void){}
};

//-----------------------------------------------------------------
class HuffmanTree
{

    HuffmanNode *root;

    //flush iobuf after compression
    int flush(char *destination);

    //flush iobuf after decompression
    int flush(char *destination,int n);

    //count the aggregate of each ASCII code
    int OpenFile(char *source);

    //return the node with the least weight
    //among the first one to the mth one
    int Get_Least(int m);

    //coding according to aggregate of each node
    void Coding(int cur,std::string temcode);

    int CreateTree(void);

    public:

    HuffmanTree(void);

    ~HuffmanTree(void);

    //copy-constructor
    HuffmanTree(const HuffmanTree &h);

    //operator =, no significance here
    HuffmanTree& operator=(const HuffmanTree& h)
    {
        //check for self-assignment
        if(&h != this)
            root = h.root;
        return *this;
    }

    int Compress(char *source,char *destination);

    int Decompress(char *source,char *destination);
};

void PrintUsage(char* s);//print help infomation

#endif
