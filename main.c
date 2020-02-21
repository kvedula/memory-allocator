#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define HEAP_SIZE 127

typedef struct Header
{ 
    int isAllocated; //0: free, 1: allocated
    size_t size; //leave room for header
} Header; 

union MemoryBlock
{
    struct Header header; //call heap[index].header.size/isAllocated
    unsigned char data; //call heap[index].data
};

union MemoryBlock heap[HEAP_SIZE];

void initializeHeap()
{
    union MemoryBlock memoryBlock;
    struct Header h = memoryBlock.header;
    unsigned char d = memoryBlock.data;

    //initialize memory block and heap
    h.size = HEAP_SIZE-1;
    h.isAllocated = 0;
    d = 0;

    //header
    heap[0] = (union MemoryBlock) h;

    //payload/data
    int i;
    for (i = 1; i < h.size; i++)
    {
        heap[i] = (union MemoryBlock) d;
    }
}

int getBestFit(size_t bytes)
{
    int bestIndex = INT_MAX;
    int i;
    for (i = 0; i < HEAP_SIZE; i++)
    {
        if (heap[i].header.isAllocated == 0 && heap[i].header.size != 0)
        {
            if (heap[i].header.size >= bytes
            && heap[i].header.size < bestIndex)
            {
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}

int getNumBytesAllocated()
{
    int numBytesAllocated = 0;
    int i;
    for (i = 0; i < HEAP_SIZE; i++)
    {
        if (heap[i].header.isAllocated == 1 && heap[i].header.size != 0)
        {
            numBytesAllocated += (heap[i].header.size + 1);
        }
    }
    return numBytesAllocated;
}

void handleBlockList()
{
    int i;
    for (i = 0; i < HEAP_SIZE; i++)
    {
        if (heap[i].header.isAllocated == 0 && heap[i].header.size != 0)
        {
            printf("%d, %lu, free\n", i+1, heap[i].header.size);
        }

        else if (heap[i].header.isAllocated == 1 && heap[i].header.size != 0)
        {
            printf("%d, %lu, allocated\n", i+1, heap[i].header.size);
        }
    }
}

void handleMalloc()
{
    char numStr[10];
    scanf("%s", numStr);
    size_t bytes = atoi(numStr);
    int headerIndex = getBestFit(bytes);
    int payload = headerIndex + 1;

    printf("%d\n", payload);

    heap[headerIndex].header.isAllocated = 1;
    heap[headerIndex].header.size = bytes;

    int newHeaderIndex = headerIndex + bytes + 1;
    int remainingBytes = HEAP_SIZE-getNumBytesAllocated();

    heap[newHeaderIndex].header.isAllocated = 0;
    heap[newHeaderIndex].header.size = remainingBytes-1; //-1 for header

    //NEED TO SPLIT BLOCK IF BYTES < ALL BLOCKS IN HEAP
}

void handleFree()
{
    char numStr[10];
    scanf("%s", numStr);
    int payloadIndex = atoi(numStr);
    int headerIndex = payloadIndex-1;
    int nextBlockHeaderIndex = headerIndex + heap[headerIndex].header.size + 1;
    if (heap[headerIndex].header.isAllocated == 1 && heap[headerIndex].header.size != 0)
    {

        if (heap[nextBlockHeaderIndex].header.isAllocated == 1 && heap[headerIndex].header.size != 0)
        {
            heap[headerIndex].header.isAllocated = 0;
            // heap[headerIndex].header.size = 0;
        }

        else if (heap[nextBlockHeaderIndex].header.isAllocated == 0 && heap[headerIndex].header.size != 0)
        {
            heap[nextBlockHeaderIndex].header.isAllocated = 0;
            heap[nextBlockHeaderIndex].header.size = 0;
            heap[headerIndex].header.isAllocated = 0;

            int remainingBytes = HEAP_SIZE-getNumBytesAllocated();
            heap[headerIndex].header.size = remainingBytes-1;
        }
    }

    if (getNumBytesAllocated() == 0)
    {
        int i;
        for (i = 0; i < HEAP_SIZE; i++)
        {
            heap[i].header.isAllocated = 0;
            heap[i].header.size = 0;
            heap[i].data = 0;
        }
        initializeHeap();
    }
}

void handleWriteMem()
{
    char numStr[10];
    char characters[200];
    scanf("%s", numStr);
    scanf("%s", characters);
    //wat is this the index for???
    int index = atoi(numStr);
    int numChars = 0;
    while (characters[numChars] != '\0')
    {
        numChars++;
    }
    int i;
    for (i = 0; i < numChars; i++)
    {
        heap[index].data = characters[i];
        index++;
    }

    //ask if i need to check stuff for this
}

void handlePrintMem()
{
    char numStr1[10];
    char numStr2[10];
    scanf("%s", numStr1);
    scanf("%s", numStr2);
    int index = atoi(numStr1);
    int numAddresses = atoi(numStr2);
    int i;
    for (i = index; i < index+numAddresses; i++)
    {
        printf("%x ", heap[i].data);
    }
    printf("\n");
}

int main()
{
    char command[50];
    initializeHeap();

    while(strcmp(command, "quit") != 0)
    {
        printf(">");
        scanf("%s", command);

        if (strcmp(command, "malloc") == 0)
        {
            handleMalloc();
        }

        else if (strcmp(command, "free") == 0)
        {
            handleFree();
        }

        else if (strcmp(command, "blocklist") == 0)
        {
            handleBlockList();
        }

        else if (strcmp(command, "writemem") == 0)
        {
            handleWriteMem();
        }

        else if (strcmp(command, "printmem") == 0)
        {
            handlePrintMem();
        }
    }
}