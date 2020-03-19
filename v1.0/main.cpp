/*------------------------------------------------------------
 //
 //  NESPAK v1.0
 //
 //  Created by Jarrod Parkes on 12/10/12.
 //  Copyright (c) 2012 Jarrod Parkes. All rights reserved.
 //
 //  Release Candidates
 //  v1.0 - 12/14/12 - Added RLE/Huffman compressions
 //
 //--------------------------------------------------------*/

/* Includes */
#include "rle.h"
#include "file.h"
#include "debug.h"
#include "huffman.h"

/* Function Prototyopes */
void HelpText(void);
void Credits(void);
void RunRlePack(char *inFilePath, char *outFilePath);
void RunRleUnpack(char *inFilePath, char *outFilePath);
void RunLz78Pack(char *inFilePath, char *outFilePath);
void RunLz78Unpack(char *inFilePath, char *outFilePath);
void RunHuffmanPack(char *inFilePath, char *outFilePath);
void RunHuffmanUnpack(char *inFilePath, char *outFilePath);

using namespace std;

/* Globals */
vector<uint8_t> unpackedData;
vector<uint8_t> packedData;
HuffTree huffTree;
vector<uint8_t> packedHuffTree;

/*------------------------------------------------------------
 //
 //  main
 //
 //  argc    - number of command line arguments (includes "NESPAK")
 //  argv    - values of command line arguments
 //
 //  Parses command line arguments, runs specified behavior.
 //
 //---------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // user inputs invalid parameters or needs help/credits
    if (argc <= 2 || argc > 5) 
    {
        if(argc == 2) 
        {
            // NESPAK help
            if((strncmp(argv[1],"help", 4) == 0) && (strlen(argv[1]) == 4))
            {
                HelpText();
            }
            // NESPAK credits
            else if((strncmp(argv[1],"credits", 7) == 0) && (strlen(argv[1]) == 7))
            {
                Credits();
            }
            // invalid 2 parameters
            else
            {
                printf("%% Invalid parameter(s).\nType 'help' for program commands.\n");
            }
        }
        // invalid paramters less than 2 or greater than 5 
        else
        {
            printf("%% Invalid parameter(s).\nType 'help' for program commands.\n");
        }
        return 0;
    }
    // 4 arugments, run program...
    else 
    {
        if((strncmp(argv[1],"rle", 3) == 0) && (strlen(argv[1]) == 3))
        {
            if((strncmp(argv[2],"pack", 4) == 0) && (strlen(argv[2]) == 4))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running RLE Pack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunRlePack(argv[3], argv[4]);
            }
            if((strncmp(argv[2],"unpack", 6) == 0) && (strlen(argv[2]) == 6))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running RLE Unpack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunRleUnpack(argv[3], argv[4]);
            }
        }
        else if((strncmp(argv[1],"lz78", 4) == 0) && (strlen(argv[1]) == 4))
        {
            if((strncmp(argv[2],"pack", 4) == 0) && (strlen(argv[2]) == 4))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running LZ78 Pack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunLz78Pack(argv[3], argv[4]);
            }
            if((strncmp(argv[2],"unpack", 6) == 0) && (strlen(argv[2]) == 6))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running LZ78 Unpack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunLz78Unpack(argv[3], argv[4]);
            }
        }
        else if((strncmp(argv[1],"huffman", 7) == 0) && (strlen(argv[1]) == 7))
        {
            if((strncmp(argv[2],"pack", 4) == 0) && (strlen(argv[2]) == 4))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running Huffman Pack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunHuffmanPack(argv[3], argv[4]);
            }
            if((strncmp(argv[2],"unpack", 6) == 0) && (strlen(argv[2]) == 6))
            {
#if(DEBUG_DEFAULT == 1)
                printf("Running Huffman Unpack...\n");
                printf("%-*s%s\n", 13, "Input file: ", argv[3]);
                printf("%-*s%s\n\n", 13, "Output file: ", argv[4]);
#endif
                RunHuffmanUnpack(argv[3], argv[4]);
            }
        }
        // invalid 5 parameters
        else
        {
            printf("%% Invalid parameter(s).\nType 'help' for program commands.\n");
        }
    }
    return 0;
}

/*------------------------------------------------------------
 //
 //  HelpText
 //
 //  Displays the NESPAK command set and options.
 //
 //---------------------------------------------------------*/
void HelpText(void)
{
    printf("----------\n");
    printf("| NESPAK |\n");
    printf("----------\n");
    printf("Command Set:\n");
    printf("%-*s", 40, "rle <method> <inFile> <outFile>");
    printf("- pack/unpack data using RLE\n");
    printf("%-*s", 40, "lz78 <method> <inFile> <outFile>");
    printf("- pack/unpack data using LZ78\n");
    printf("%-*s", 40, "help");
    printf("- display help menu\n");
    printf("%-*s", 40, "credits");
    printf("- display credits\n\n");
    printf("Arguments:\n");
    printf("%-*s", 40, "method");
    printf("- pack/unpack\n");
    printf("%-*s", 40, "inFile");
    printf("- path to nametable\n");
    printf("%-*s", 40, "outFile");
    printf("- desired path for output\n\n");
    printf("\n");
}

/*------------------------------------------------------------
 //
 //  Credits
 //
 //  Displays the NESPAK version numbers and author.
 //
 //---------------------------------------------------------*/
void Credits(void)
{
    // header
    printf("----------\n");
    printf("| NESPAK |\n");
    printf("----------\n");
    printf("%-*s", 20, "Author: ");
    printf("Jarrod Parkes\n");
    printf("%-*s", 20, "Date: ");
    printf("12/20/12\n");
    printf("%-*s", 20, "Version: ");
    printf("v1.0\n\n");
    
    // release notes
    printf("Release Notes: \n");
    printf("%-*s", 10, "v1.0");
    printf("- added RLE/Huffman compressions\n");
}

/*------------------------------------------------------------
 //
 //  RunRlePack
 //
 //  inFilePath      - path to binary input file
 //  outFilePath     - path to binary output file
 //
 //  Packs binary input file into output file using RLE.
 //
 //---------------------------------------------------------*/
void RunRlePack(char *inFilePath, char *outFilePath)
{
    if(ReadBinaryFile(inFilePath, &unpackedData))
    {
        PackRle(&unpackedData, &packedData);
    }
    else
    {
        printf("%% Invalid input file.\n");
        return;
    }
    if(!WriteBinaryFile(outFilePath, &packedData))
    {
        printf("%% Invalid output file\n");
        return;
    }
    return;
}

/*------------------------------------------------------------
 //
 //  RunRleUnpack
 //
 //  inFilePath      - path to binary input file
 //  outFilePath     - path to binary output file
 //
 //  Unpacks binary input file into output file using RLE.
 //
 //---------------------------------------------------------*/
void RunRleUnpack(char *inFilePath, char *outFilePath)
{
    if(ReadBinaryFile(inFilePath, &packedData))
    {
        UnpackRle(&packedData, &unpackedData);
    }
    else
    {
        printf("%% Invalid input file.\n");
        return;
    }
    if(!WriteBinaryFile(outFilePath, &unpackedData))
    {
        printf("%% Invalid output file\n");
        return;
    }
    return;
}

/*------------------------------------------------------------
 //
 //  RunLz78Pack
 //
 //  inFilePath      - path to binary input file
 //  outFilePath     - path to binary output file
 //
 //  Packs binary input file into output file using LZ78.
 //
 //---------------------------------------------------------*/
void RunLz78Pack(char *inFilePath, char *outFilePath)
{
    printf("%% LZ78 Compression: Not yet implemented!");
}

/*------------------------------------------------------------
 //
 //  RunLz78Unpack
 //
 //  inFilePath      - path to binary input file
 //  outFilePath     - path to binary output file
 //
 //  Unpacks binary input file into output file using LZ78.
 //
 //---------------------------------------------------------*/
void RunLz78Unpack(char *inFilePath, char *outFilePath)
{
    printf("%% LZ78 Decompression: Not yet implemented!");
}

/*------------------------------------------------------------
 //
 //  RunHuffmanPack
 //
 //  inFilePath      - path to ASCII input file
 //  outFilePath     - path to binary output file
 //
 //  Packs ASCII input file into output file using Huffman.
 //
 //---------------------------------------------------------*/
void RunHuffmanPack(char *inFilePath, char *outFilePath)
{
    if(!IsAscii(inFilePath))
    {
        return;
    }
    else
    {
        if(ReadAsciiFile(inFilePath, huffTree))
        {
#if(DEBUG_VERBOSE == 1)
            PrintHuffTreeBFS(huffTree);
#endif
            if(PackHuffman(inFilePath, huffTree, &packedData))
            {
                PrintHuffmanFinal(&packedData);
#if(DEBUG_VERBOSE == 1)
                UnpackHuffman(huffTree, &packedData);
#endif
            }
            else
            {
                printf("%% Could not compress file using Huffman.\n");
            }
        }
        else
        {
            printf("%% Reading ASCII file failure.\n");
        }
        if(!WriteBinaryFile(outFilePath, &packedData))
        {
            printf("%% Could not write output file.\n");
            return;
        }
    }
    return;
}

/*------------------------------------------------------------
 //
 //  RunHuffmanUnpack
 //
 //  inFilePath      - path to binary input file
 //  outFilePath     - path to ASCII output file
 //
 //  Unpacks binary input file into ASCII output file using 
 //  Huffman.
 //
 //  TO-DO: Also encode the Huffman tree into file
 //
 //---------------------------------------------------------*/
void RunHuffmanUnpack(char *inFilePath, char *outFilePath)
{
    printf("%% Huffman Decompression: Not yet implemented!\n");
}


