/*----------------------------------------------------------
 //
 //  rle.h
 //
 //  Defines prototypes and constants for RLE. 
 //  RLE "Run-Length Encoding" is a simple compression in 
 //  the following form:
 //
 //  Unpacked : AAAAAFFFFAF
 //  Packed   : 5A 4F -2AF
 //  NES Form : 0x05 0x65 0x04 0x70 0xFF 0x02 0x65 0x70
 //
 //  * 0xFF indicates a unique run, followed by a
 //    length byte to indicated run length
 //
 //--------------------------------------------------------*/

#ifndef RLE_H
#define RLE_H

/* Includes */
#include <vector>
#include "sys_def.h"

/* Defines */
#define RLE_MAX_RUN_SIZE 255        // maximum RLE run size, because NES is 8-bit, reserves 0xFF for unique run
#define RLE_UNIQUE_RUN_START 0xFF   // denotes unique run

using namespace std;

/* Function Prototyopes */
void PackRle(vector<uint8_t> *unpackedData, vector<uint8_t> *packedData);
void UnpackRle(vector<uint8_t> *packedData, vector<uint8_t> *unpackedData);

#endif
