/*----------------------------------------------------------
 //
 //  file.h
 //
 //  Defines function prototypes for file I/O.
 //
 //--------------------------------------------------------*/

#ifndef FILE_H
#define FILE_H

/* Includes */
#include "huffman.h"
#include "sys_def.h"

using namespace std;

/* Function Prototyopes */
bool ReadBinaryFile(const char *inputPath, vector<uint8_t> *unpackedData);
bool WriteBinaryFile(const char *inputPath, vector<uint8_t> *packedData);
bool IsAscii(const char *inputPath);
bool ReadAsciiFile(const char *inputPath, HuffTree &huffTree);

#endif
