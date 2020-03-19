/*----------------------------------------------------------
 //
 //  file.cpp
 //
 //  Implements functions for file I/O.
 //
 //--------------------------------------------------------*/

/* Includes */
#include <fstream>
#include "file.h"
#include "debug.h"
#include "huffman.h"

/*------------------------------------------------------------
 //
 //  ReadBinaryFile
 //
 //  filePath       - path to binary input file
 //  unpackedData   - array to store bytes read from file
 //
 //  Reads and stores binary data from file.
 //
 //---------------------------------------------------------*/
bool ReadBinaryFile(const char *filePath, vector<uint8_t> *unpackedData)
{
    ifstream inFile;
    ifstream::pos_type inSize;
    char *data = NULL;
    
    inFile.open(filePath, ios::in|ios::binary|ios::ate);
    
    if(inFile.is_open())
    {
        inSize = inFile.tellg();                // get size of file
        data = new char[(unsigned int)inSize];  // create memory for file contents
        inFile.seekg(0, ios::beg);              // move ptr to beginning of file
        inFile.read(data, inSize);
        
        // convert data
        for(int i = 0; i < inSize; i++)
        {
            unpackedData->push_back((uint8_t)data[i]);
        }
    }
    else
    {
        inFile.close();
        return false;
    }
    
    delete[] data;
    inFile.close();
    return true;
}

/*------------------------------------------------------------
 //
 //  WriteBinaryFile
 //
 //  filePath       - path to binary output file
 //  packedData     - array of bytes to output
 //
 //  Writes binary data to output file.
 //
 //---------------------------------------------------------*/
bool WriteBinaryFile(const char *filePath, vector<uint8_t> *packedData)
{
    ofstream outFile;
    outFile.open(filePath, ios::out|ios::binary|ios::ate);
    
    if(outFile.is_open())
    {
        for(unsigned int i = 0; i < packedData->size(); ++i)
        {
            outFile.put(packedData->at(i));
        }        
    }
    else
    {
        outFile.close();
        return false;
    }
    
    outFile.close();
    return true;
}

/*------------------------------------------------------------
 //
 //  IsAscii
 //
 //  filePath       - path to input file
 //
 //  Verifies input file is made of printable ASCII characters
 //
 //---------------------------------------------------------*/
bool IsAscii(const char *filePath)
{
    ifstream inFile;
    ifstream::pos_type inSize;
    char *inData = NULL;
    
    inFile.open(filePath, ios::in|ios::binary|ios::ate);
    
    if(inFile.is_open())
    {
        inSize = inFile.tellg();                    // get size of file
        inData = new char[(unsigned int)inSize];    // create memory for file contents
        inFile.seekg(0, ios::beg);                  // move ptr to beginning of file
        
        if(!inFile.read(inData, inSize))
        {
            printf("%% Could not read file.");
            delete[] inData;
            inFile.close();
            return false;
        }
        
        else
        {
            for(int i = 0; i < inSize; i++)
            {
                //ignore newline characters and EOF
                if(inData[i] == '\n' || inData[i] == EOF)
                {
                    continue;
                }

                if(!isprint(inData[i]))
                {
                    printf("%% Input file not ASCII.");
                    delete[] inData;
                    inFile.close();
                    return false;
                }
            }
        }
    }
    else
    {
        printf("%% Could not open file.");
        inFile.close();
        return false;
    }

    delete[] inData;
    inFile.close();
    return true;
}

/*------------------------------------------------------------
 //
 //  ReadAsciiFile
 //
 //  filePath       - path to input file
 //  huffTree       - Huffman tree to be created
 //
 //  Creates Huffman tree based on frequency of encountered
 //  ASCII characters seen in the input file.
 //
 //---------------------------------------------------------*/
bool ReadAsciiFile(const char* filePath, HuffTree &huffTree)
{
    char *inData = NULL;
    
    ifstream inFile;
    ifstream::pos_type inSize;
    inFile.open(filePath, ios::in|ios::binary|ios::ate);
    
    vector<HuffNode *> psuedoStack;
    HuffNode *node = NULL;

    // create a HuffNode for each ASCII value
    for(int i = 0; i < ASCII_MAX_VALUE; ++i)
    {
        node = new HuffNode();
        node->symbol = (char)i;
        node->count = 0;
        node->left = node->right = node->parent = NULL;
        psuedoStack.push_back(node);
    }
   
    // open file and read each character, recording their counts
    if(inFile.is_open())
    {
        inSize = inFile.tellg();                    // get size of file
        inData = new char[(unsigned int)inSize];    // create memory for file contents
        inFile.seekg(0, ios::beg);                  // move ptr to beginning of file
        
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
                psuedoStack[inData[i]]->count++;
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
  
    // remove nodes that did not occur, they will not be in the Huffman tree
    for(vector<HuffNode*>::iterator it=psuedoStack.begin(); it != psuedoStack.end();)
    {
        if((*it)->count == 0)
        {
            psuedoStack.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    // sort the HuffNodes in descending order based on how many times symbol occured
    SortHuffNodes(psuedoStack);

    // now build the Huffman tree!
    HuffNode *nodeA, *nodeB;
    huffTree.head = NULL;
    huffTree.size += psuedoStack.size();

    // step 1 - removed two least frequent nodes "top of stack"
    // step 2 - create a new node that combines the frequencies/symbols of nodes from step 1
    // step 3 - make the new node the parent of the nodes from step 1 "creating tree structure"
    // step 4 - insert the new parent node back into the stack based on combined frequency
    // step 5 - continue steps 1-4 until only one node is left "root of the tree"
    while(psuedoStack.size() > 1)
    {
        vector<HuffNode*>::iterator it;
        nodeA = psuedoStack.back();
        psuedoStack.pop_back();
        nodeB = psuedoStack.back();
        psuedoStack.pop_back();
        
        HuffNode *parent = new HuffNode();
        parent->count = nodeA->count + nodeB->count;
#if(DEBUG_VERBOSE == 1)
        printf("Combining %s and %s into a new node\n", nodeA->symbol.c_str(), nodeB->symbol.c_str());
#endif
        parent->symbol = "";
        parent->symbol.append(nodeA->symbol);
        parent->symbol.append(nodeB->symbol);
        parent->left = nodeA;
        parent->right = nodeB;
        nodeA->parent = nodeB->parent = parent;
        huffTree.size++;
        
        // insert the parent node back into the "stack"
        for(it=psuedoStack.begin(); it != psuedoStack.end(); ++it)
        {
            if((*it)->count < parent->count)
            {
                psuedoStack.insert(it,parent);
                break;
            }
        }
        
        // iterator made it to the end of the vector, push parent to the end 
        if(it == psuedoStack.end())
        {
            psuedoStack.push_back(parent);
        }
    }
    
    // pop the last element "root of the tree"
    huffTree.head = psuedoStack.back();
    
#if(DEBUG_VERBOSE == 1)
    printf("\n");
#endif
    
    return true;
}
