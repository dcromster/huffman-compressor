//Huf.cpp
//implementation of functions in Huffman.h

#include "Huffman.h"
using namespace std;

/* Max code(255) + 1*/
const int MAX_VALUE = 256;	

/* keep the weights of 256 ASCII code */
int weight[256] = {0};		

/* if index == LEFT,then it's the left child */
enum {LEFT = 1,RIGHT};		

string iobuf;		
int padding = 0;		

//copy-constructor
HuffmanTree::HuffmanTree(const HuffmanTree &h){}

HuffmanTree::HuffmanTree(void)
{
    root = new HuffmanNode[MAX_VALUE*2-1];
}

HuffmanTree::~HuffmanTree(void){delete []root;}

int HuffmanTree::Get_Least(int m)
{
    //This is of very low efficience.
    //It could be changed to binomial heap.
    int a = MAX_VALUE*2-1;
    for (int i = 0; i < m; ++i)
        if ((root[a].weight > root[i].weight) && (root[i].parent == 0))
            a = i;
    return a;
}

int HuffmanTree::CreateTree(void)
{
    //create tree according to ASCII code
    int s1,s2;
    for (int i = MAX_VALUE; i < MAX_VALUE*2-1; ++i)
    {
        s1 = Get_Least(i);
        root[s1].parent = i;
        root[s1].index = LEFT;
        s2 = Get_Least(i);			
        root[s2].parent = i;
        root[s2].index = RIGHT;
        root[i].left = s1;
        root[i].right = s2;
        root[i].weight = root[s1].weight + root[s2].weight;
    }
    Coding(MAX_VALUE*2-2,"");//call Coding() recursivelly

    return 0;
}

void HuffmanTree::Coding(int cur,string temcode)
{
    if (cur < MAX_VALUE)
    {
        root[cur].code = temcode;
        return;
    }
    Coding(root[cur].left, temcode + "1");
    Coding(root[cur].right, temcode + "0");
}

int HuffmanTree::flush(char *destination)
{
    ofstream out(destination,ios::binary);//binary mode,this is very important!
    if(!out)
    {
        perror(destination);
        return 1;
    }
    out << padding << " ";
    for(int k = 0; k < MAX_VALUE; ++k)
        out << weight[k] << " ";
    string::size_type i = 0,j;
    while(i < iobuf.length())
    {
        char c = 0;
        //read 8 bits,output as one single ASCII code
        for(j = 0; j < 8; ++j)
            c += (char)pow(2.0,7.0-j)*(iobuf[i++] - '0');
        out.write(&c,1);
    }
    out.close();
    return 0;
}

//--------------------overload flush----------------------
int HuffmanTree::flush(char *destination, int n)
{
    ofstream out(destination,ios::binary);
    if(!out)
    {
        perror(destination);
        return 1;
    }

    char c = 0;
    int h = MAX_VALUE*2-2;
    string::size_type i = 0;

    while(i <= iobuf.length() - padding)/*delete the padding 0s*/
    {
        //find the leaf node
        if(h >= MAX_VALUE)
        {
            if(iobuf[i] == '1')
                h = root[h].left;
            else
                h = root[h].right;
            ++i;
        }
        else{//found
            c = h;
            out.write(&c,1);
            h = MAX_VALUE*2-2;
        }
    }
    out.close();
    return 0;
}

/* 
 * read the given file for the first time and count the frequentness of each
 * ASCII code
 */
int HuffmanTree::OpenFile(char *source)
{
    ifstream in(source,ios::binary);
    if (! in) 
    {
        perror(source);
        return 1;
    }
    streamoff length;
    int i,tmp;
    char * buffer;

    // get length of file:
    in.seekg (0, ios::end);
    length = in.tellg();
    in.seekg (0, ios::beg);

    buffer = new char [(int)length];
    in.read (buffer,length);
    in.close();

    for(i=0; i<length; ++i)
    {
        tmp = (unsigned char)buffer[i];
        weight[tmp]++;
    }

    //initialize the weight of each leaf node
    for (int i = 0; i < MAX_VALUE; ++i)
        root[i].weight = weight[i];

    root[MAX_VALUE*2-1].weight = INT_MAX;
    root[MAX_VALUE*2-2].index = -1;

    delete[] buffer;
    return 0;
}

int HuffmanTree::Compress(char *source,char *destination)
{
    cout << "Compressing " << source << " to " << destination << endl;
    int ret;

    ret = OpenFile(source);
    if(ret != 0)
    {
        cerr << "Open file failed\n";
        exit(1);
    }
    ret = CreateTree();
    if(ret != 0)
    {
        cerr << "Create tree failed\n";
        exit(1);
    }

    ifstream in(source,ios::binary);
    if(!in)
    {
        perror(source);
        return 1;
    }

    char c;
    while(in.read(&c, 1)){
        iobuf += root[(unsigned char)c].code;
    }
    in.close();

    //calculate padding, add 0s to iobuf at the end
    padding = 8 - iobuf.size() % 8;
    for(int i = 0; i < padding; ++i)
        iobuf += "0";

    flush(destination);
    cout << "Compression finished\n";

    return 0;
}

int HuffmanTree::Decompress(char *source,char *destination)
{
    cout << "Decompressing " << source << " to " << destination << endl;

    ifstream in(source,ios::binary);
    if(!in)
    {
        perror(source);
        return 1;
    }

    in >> padding;

    char c;
    for(int i = 0; i < MAX_VALUE; ++i)
        in >> root[i].weight;

    root[MAX_VALUE*2-1].weight = INT_MAX;
    root[MAX_VALUE*2-2].index = -1;

    in.read(&c,1);
    while(in.read(&c,1))
        for(int i = 0; i < 8; ++i)
            iobuf += (c & 0x80 >> i)? "1" : "0";

    CreateTree();

    flush(destination,1);
    cout << "Decompression finished\n";

    return 0;
}

void PrintUsage(char* s)
{
    std::cout << "----------------------------------------------------\n"
        << "Usage:\t" << s << " [-c]/[-d] [source] [destination]\n"
        << "e.g.\t" << s << " -c ~/universe ~/atom\n"
        << "-c:\tcompress source file to destination\n"
        << "-d:\tdecompress source file to destination\n"
        << "----------------------------------------------------\n";
}
