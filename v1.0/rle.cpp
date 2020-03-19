/*----------------------------------------------------------
 //
 //  rle.cpp
 //
 //  Implementations for RLE. RLE "Run-Length Encoding" is a 
 //  simple compression in the following form:
 //
 //  Unpacked : AAAAAFFFFAF
 //  Packed   : 5A 4F -2AF
 //  NES Form : 0x05 0x65 0x04 0x70 0xFF 0x02 0x65 0x70
 //
 //  * 0xFF indicates a unique run, followed by a
 //    length byte to indicated run length
 //
 //--------------------------------------------------------*/

/* Includes */
#include "rle.h"
#include "debug.h"

/*------------------------------------------------------------
 //
 //  PackRle
 //
 //  unpackedData    - array of bytes
 //  packedData      - array of bytes to be packed
 //
 //  Packs unpackedData into RLE form.
 //
 //---------------------------------------------------------*/
void PackRle(vector<uint8_t> *unpackedData, vector<uint8_t> *packedData)
{
    uint8_t uniqueQueue[RLE_MAX_RUN_SIZE];
    uint8_t queueIndex = 0;
    
    uint8_t pattern = unpackedData->at(0);
    uint8_t count = 0;
    
    for(unsigned int i = 0; i < unpackedData->size(); ++i)
    {
        // we've started a pattern
        if(unpackedData->at(i) == pattern)
        {
            count++;
            
            // the pattern is greater than 1
            if(count == 2)
            {
                // have we previously encountered a unique run of values?
                if(queueIndex > 0)
                {
                    // we have a unique run, indicate with (0xFF)
                    packedData->push_back(RLE_UNIQUE_RUN_START);
                    // how long is the unique run?
                    packedData->push_back(queueIndex);
                    // now, empty unique queue
                    for(int i = 0; i < queueIndex; ++i)
                    {
                        packedData->push_back(uniqueQueue[i]);
                    }
                }
                queueIndex = 0;
            }
            
            // our run has reached mamimum size
            if(count == RLE_MAX_RUN_SIZE)
            {
                packedData->push_back(count);
                packedData->push_back(pattern);
                pattern = unpackedData->at(i);
                count = 1;
            }
        }
        // not a pattern
        else
        {
            if(count > 1)
            {
                packedData->push_back(count);
                packedData->push_back(pattern);
            }
            else
            {
                uniqueQueue[queueIndex++] = unpackedData->at(i-1);
            }
            pattern = unpackedData->at(i);
            count = 1;
        }
    }
    
    packedData->push_back(count);
    packedData->push_back(pattern);
    
#if(DEBUG_DEFAULT == 1)
    printf("Status Report...\n");
    printf("%-*s%lu bytes\n", 15, "Before: ", unpackedData->size());
    printf("%-*s%lu bytes\n", 15, "After: ", packedData->size());
    printf("%-*s%.2f%%\n", 15, "Compression: ", ((float)((unpackedData->size() - packedData->size())/(float)unpackedData->size())) * 100);
#endif
    
    return;
}

/*------------------------------------------------------------
 //
 //  UnpackRle
 //
 //  packedData     - array of compressed bytes
 //  unpackedData   - array of bytes to be unpacked
 //
 //  Unpacks packedData into unpacked form.
 //
 //---------------------------------------------------------*/
void UnpackRle(vector<uint8_t> *packedData, vector<uint8_t> *unpackedData)
{
    uint8_t count = 0;
    uint8_t nextByte = 0;
    
    for(unsigned int i = 0; i < packedData->size(); ++i)
    {
        nextByte = packedData->at(i);
        
        if(nextByte == RLE_UNIQUE_RUN_START)    // started a unique run 0xFF
        {
            count = packedData->at(++i);
            for(int j = 0; j < count; ++j)
            {
                unpackedData->push_back(packedData->at(++i));
            }
        }
        else    // started a normal run
        {
            count = nextByte;
            i++;
            for(int j = 0; j < count; ++j)
            {
                unpackedData->push_back(packedData->at(i));
            }
        }
    }
    
#if(DEBUG_DEFAULT == 1)
    printf("Status Report...\n");
    printf("%-*s%lu bytes\n", 15, "Before: ", packedData->size());
    printf("%-*s%lu bytes\n", 15, "After: ", unpackedData->size());
#endif
    
    return;
}