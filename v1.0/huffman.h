/*----------------------------------------------------------
 //
 //  huffman.h
 //
 //  Defines prototypes, structs, and constants for Huffman.
 //  For a description about Huffman compression, see
 //  the following link:
 //
 //  www.youtube.com/watch?v=0PahtaFK640
 //
 //--------------------------------------------------------*/

#ifndef HUFFMAN_H
#define HUFFMAN_H

/* Includes */
#include <vector>
#include <string>
#include "sys_def.h"

/* Defines */
#define ASCII_MAX_VALUE 256

using namespace std;

/* HuffNode */
typedef struct HuffNode
{
    string symbol;
    int count;
    HuffNode *left;
    HuffNode *right;
    HuffNode *parent;
} HuffNode;

/* HuffTree */
typedef struct HuffTree
{
    HuffNode *head;
    int size;
} HuffTree;

/* Function Prototyopes */
void SortHuffNodes(vector<HuffNode *> nodes);
void PrintHuffTreePreOrder(HuffTree &tree);
void PrintHuffNode(HuffNode *node, bool isLeftChild);
void PrintHuffTreeBFS(HuffTree &tree);
bool PackHuffman(const char *filePath, HuffTree &huffTree, vector<uint8_t> *packedData);
void PackHuffValue(char value, HuffTree &huffTree, vector<uint8_t> *packedData);
void WriteBit(bool bit, vector<uint8_t> *packedData);
void WriteByte(uint8_t byte, vector<uint8_t> *packedData);
uint8_t FlipBits(uint8_t byte);
void PrintHuffmanFinal(vector<uint8_t> *packedData);
void PrintByte(uint8_t byte);
void UnpackHuffman(HuffTree &huffTree, vector<uint8_t> *packedData);

#endif
