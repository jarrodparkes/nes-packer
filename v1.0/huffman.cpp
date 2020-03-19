/*----------------------------------------------------------
 //
 //  huffman.cpp
 //
 //  Implements Huffman compression. For a description about 
 //  Huffman compression, see the following link:
 //
 //  www.youtube.com/watch?v=0PahtaFK640
 //
 //--------------------------------------------------------*/

/* Includes */
#include <queue>
#include <fstream>
#include "debug.h"
#include "huffman.h"

using namespace std;

/* Globals */
static uint8_t buffByte = 0;
static int accumulator = 0;
static int beforeBytes = 0;

/* Flip 4-Bit LUT */
uint8_t four_bit_lut[16] = {
    0x0, 0x8, 0x4, 0xC,
    0x2, 0xA, 0x6, 0xE,
    0x1, 0x9, 0x5, 0xD,
    0x3, 0xB, 0x7, 0xF
};

/*------------------------------------------------------------
 //
 //  SortHuffNodes
 //
 //  nodes      - vector of Huffman nodes
 //
 //  Sorts nodes in decreasing order so that vector emulates
 //  a stack with the highest frequency nodes at the bottom
 //  and lowest frequency nodes at the top. Sorts using
 //  insertion sory method.
 //
 //---------------------------------------------------------*/
void SortHuffNodes(vector<HuffNode *> nodes)
{
    unsigned int i = 0;
    int j = 0;

    for(i = 1; i < nodes.size(); ++i)
    {
        HuffNode *value = nodes.at(i);

        for(j = i - 1; j >= 0; j--)
        {
            if(value->count > nodes.at(j)->count)
            {
                nodes.at(j+1) = nodes.at(j);
            }
            else
            {
                break;
            }
        }
        nodes.at(j+1) = value;
    }
}

/*------------------------------------------------------------
 //
 //  PrintHuffTreeBFS
 //
 //  tree      - a Huffman tree
 //
 //  Prints Huffman tree using BFS traversal. Resulting
 //  output shows tree organized by levels.
 //
 //---------------------------------------------------------*/
void PrintHuffTreeBFS(HuffTree &huffTree)
{
    queue<HuffNode *> q;
    q.push(huffTree.head);
    
    int printsTillNewLine = 1;
    int printsBuffer = 2;
    int level = 0;
    int size = huffTree.size;
    
    printf("-------------------------------------\n");
    printf("| Huffman Tree Breadth First Search |\n");
    printf("-------------------------------------\n");
    printf("%-*s", 10, "Root: ");
    
    while(!q.empty())
    {
        printf("\"%s\"   ", q.front()->symbol.c_str());
        printsTillNewLine--;
        size--;
        
        if(printsTillNewLine == 0 && size > 1)
        {
            printf("\nLevel %d:  ", ++level);
            printsTillNewLine = printsBuffer;
            printsBuffer = 0;
        }
        
        // you only need to check one child (because this is a 2-tee)
        if(q.front()->left != NULL)
        {
            q.push(q.front()->left);
            q.push(q.front()->right);
            printsBuffer += 2;
        }

        q.pop();
    }
    return;
}

/*------------------------------------------------------------
 //
 //  PrintHuffTreePreOrder
 //
 //  tree      - a Huffman tree
 //
 //  Prints Huffman tree using preorder traversal.
 //
 //---------------------------------------------------------*/
void PrintHuffTreePreOrder(HuffTree &huffTree)
{
    printf("Root: \"%s\"\n", huffTree.head->symbol.c_str());
    printf("%-*s%-*s%-*s\n", 30, "String", 15, "L/R Child", 30, "Parent");
    printf("%-*s%-*s%-*s\n", 30, "------", 15, "---------", 30, "------");
    PrintHuffNode(huffTree.head->left, true);
    PrintHuffNode(huffTree.head->right, false);
    return;
}

/*------------------------------------------------------------
 //
 //  PrintHuffNode
 //
 //  node           - a Huffman node
 //  isLeftChild    - is the Huffman node a left child?
 //
 //  Prints data for single Huffman node.
 //
 //---------------------------------------------------------*/
void PrintHuffNode(HuffNode *node, bool isLeftChild)
{
    if(node->left == NULL && node->right == NULL)
    {
        if(isLeftChild)
        {
            printf("\"%s\"%-*s%-*s%-*s\n", node->symbol.c_str(), 28-(int)(node->symbol.size()), "", 15, "L", 30, node->parent->symbol.c_str());
        }
        else
        {
            printf("\"%s\"%-*s%-*s%-*s\n", node->symbol.c_str(), 28-(int)(node->symbol.size()), "", 15, "R", 30, node->parent->symbol.c_str());
        }
    }
    else
    {
        if(isLeftChild)
        {
            printf("\"%s\"%-*s%-*s%-*s\n", node->symbol.c_str(), 28-(int)(node->symbol.size()), "", 15, "L", 30, node->parent->symbol.c_str());
        }
        else
        {
            printf("\"%s\"%-*s%-*s%-*s\n", node->symbol.c_str(), 28-(int)(node->symbol.size()), "", 15, "R", 30, node->parent->symbol.c_str());
        }
        PrintHuffNode(node->left, true);
        PrintHuffNode(node->right, false);
    }
}

/*------------------------------------------------------------
 //
 //  PackHuffman
 //
 //  filePath       - path to ASCII input file
 //  huffTree       - a Huffman tree
 //  packedData     - array of bytes to output
 //
 //  Reads input file and compresses each character using
 //  huffTree. Results stored in packedData.
 //
 //---------------------------------------------------------*/
bool PackHuffman(const char* filePath, HuffTree &huffTree, vector<uint8_t> *packedData)
{
    char *inData = NULL;
    
    ifstream inFile;
    ifstream::pos_type inSize;
    inFile.open(filePath, ios::in|ios::binary|ios::ate);
#if(DEBUG_VERBOSE == 1)
    printf("\n\nText    : ");
#endif
    // open file and read each character, recording their counts
    if(inFile.is_open())
    {
        inSize = inFile.tellg();        // get size of file
        inData = new char[(unsigned int)inSize];      // create memory for file contents
        inFile.seekg(0, ios::beg);      // move ptr to beginning of file
        
        if(!inFile.read(inData, inSize))
        {
            delete[] inData;
            inFile.close();
            return false;
        }
        
        else
        {
            for(int i = 0; i < inSize; i++)
            {
#if(DEBUG_VERBOSE == 1)
                printf("%c", inData[i]);
#endif
                beforeBytes += 2;
                
                // compress character
                PackHuffValue(inData[i], huffTree, packedData);
            }
            if(accumulator != 0)
            {
                WriteByte(buffByte, packedData);
            }
        }
    }
    else
    {
        inFile.close();
        return false;
    }
    
    // done with file, close it
    delete[] inData;
    inFile.close();
    return true;
}

/*------------------------------------------------------------
 //
 //  PackHuffValue
 //
 //  value          - character to be packed
 //  huffTree       - a Huffman tree
 //  packedData     - array of bytes to output
 //
 //  Given a character, the huffTree is traversed until
 //  reaching a leaf node. For each left traversal, write 0.
 //  For each right traversal, write 1.
 //
 //---------------------------------------------------------*/
void PackHuffValue(char value, HuffTree &huffTree, vector<uint8_t> *packedData)
{
    HuffNode *temp = huffTree.head;
    size_t found;
    
    // continue loop until you reach a leaf node
    while(temp->left != NULL)
    {
        found = temp->left->symbol.find(value);
        
        if(found != string::npos)   // character in the left child
        {
            WriteBit(0, packedData);
            temp = temp->left;
        }
        else    // character in the right child
        {
            WriteBit(1, packedData);
            temp = temp->right;
        }
    }    
}

/*------------------------------------------------------------
 //
 //  WriteBit
 //
 //  bit            - 0 or 1
 //  packedData     - array of bytes to output
 //
 //  Writes one bit at a time into the accumulator. Once
 //  the accumulator is full, the full byte is stored
 //  in packedData.
 //
 //---------------------------------------------------------*/
void WriteBit(bool bit, vector<uint8_t> *packedData)
{
    buffByte |= (bit ? 1 : 0) << accumulator;
    accumulator++;
    
    if (accumulator == 8)
    {
        WriteByte(buffByte, packedData);
        buffByte = 0;
        accumulator = 0;
    }
}

/*------------------------------------------------------------
 //
 //  WriteByte
 //
 //  byte           - an 8-bit value
 //  packedData     - array of bytes to output
 //
 //  Writes byte into packedData.
 //
 //---------------------------------------------------------*/
void WriteByte(uint8_t byte, vector<uint8_t> *packedData)
{
    byte = FlipBits(byte);
    packedData->push_back(byte);
}

/*------------------------------------------------------------
 //
 //  FlipBits
 //
 //  byte           - an 8-bit value
 //
 //  Reverses the order of a byte to correct output.
 //  Ex: 1001 1100 --(yields)--> 0011 1001
 //
 //---------------------------------------------------------*/
uint8_t FlipBits(uint8_t byte)
{
    // step 1 - flip the first 4 bits, using LUT
    // step 2 - flip the second 4 bits, using LUT
    // step 3 - combine step 1 & step 2 by OR operation
    return (four_bit_lut[byte & 0x0F] << 4) | four_bit_lut[byte >> 4];
}

/*------------------------------------------------------------
 //
 //  PrintHuffmanFinal
 //
 //  packedData           - array of bytes to output
 //
 //  Prints the result of the Huffman compression in
 //  multiple bases and gives final status report.
 //
 //---------------------------------------------------------*/
void PrintHuffmanFinal(vector<uint8_t> *packedData)
{
#if(DEBUG_VERBOSE == 1)
    printf("\nBinary  : ");
    for(int i = 0; i < packedData->size(); ++i)
    {
        PrintByte(packedData->at(i));
    }
    printf("\nHex     : ");
    for(int i = 0; i < packedData->size(); ++i)
    {
        printf("%#x     ", packedData->at(i));
    }
    printf("\n\n");
#endif
    
#if(DEBUG_DEFAULT == 1)
    printf("Status Report...\n");
    printf("%-*s%d bytes\n", 15, "Before: ", beforeBytes);
    printf("%-*s%lu bytes\n", 15, "After: ", packedData->size());
    printf("%-*s%.2f%%\n", 15, "Compression: ", ((float)((beforeBytes - packedData->size())/(float)beforeBytes)) * 100);
#endif
    
    return;
}

/*------------------------------------------------------------
 //
 //  PrintByte
 //
 //  byte           - an 8-bit value
 //
 //  Prints byte in binary form.
 //
 //---------------------------------------------------------*/
void PrintByte(uint8_t byte)
{
    uint8_t mask = 0x80;
    uint8_t bits[8];
    
    // extract the bits
    for (int i = 0; i < 8; ++i)
    {
        // mask each bit in the byte and store it
        bits[i] = (byte & (mask >> i)) != 0;
    }
    
    // print bits
    for(int i = 0; i < 8; ++i)
    {
        printf("%d",bits[i]);
    }
    printf(" ");
}

/*------------------------------------------------------------
 //
 //  UnpackHuffman
 //
 //  huffTree       - a Huffman tree
 //  packedData     - array of bytes packed using Huffman
 //
 //  Decompresses packedData using huffTree and prints
 //  the result.
 //
 //---------------------------------------------------------*/
void UnpackHuffman(HuffTree &huffTree, vector<uint8_t> *packedData)
{
    printf("\nUncompressed: ");
    HuffNode *node = huffTree.head;
    int left_or_right;
    uint8_t mask;
    
    for(unsigned int i = 0; i < packedData->size(); ++i)
    {
        mask = 0x80;
        while(mask != 0x00)
        {
            // is the next bit a 0 or 1?
            left_or_right = (packedData->at(i) & mask);
            
            // shift the mask one to the right
            mask = mask >> 1;
            
            // traverse left
            if(left_or_right == 0)
            {
                node = node->left;
            }
            // traverse right
            else
            {
                node = node->right;
            }
            // have we reached a leaf node?
            if(node->left == NULL)
            {
                // print symbol
                printf("%s", node->symbol.c_str());
                // reset to the top of tree
                node = huffTree.head;
            }
        }
    }
}