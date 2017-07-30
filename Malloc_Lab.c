// Oliande, Bryan: 13179240

//LAB 3 SUBMISSION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MALLOC_SIZE 400
#define HEADER_SIZE 9

static int blockID = 1;

void allocate(int numBytesWanted, char *head);
void freeheap(int blockNumToDelete, char *head);
void blocklist(char *head);
void writeheap(int blockNumToWriteInto, char charToWrite, int numChars, char *head);
void printheap(int blockNumToPrint, int numBytesToPrint, char *head);

int main()
{
    char command[20];
    char argument[20];
    int flag = 1;
    int numBytesWanted = 0;
    int blockNumToDelete = 0;
    int blockNumToWriteInto = 0;
    char charToWrite = '?';
    int numChars = 0;
    int blockNumToPrint = 0;
    int numBytesToPrint = 0;

    char *mHead = (char*)malloc(MALLOC_SIZE);

    // DEBUG
    int i;
    for (i = 0; i < MALLOC_SIZE; ++i)
        mHead[i] = NULL;

    do
    {
        printf("> ");
        scanf("%s", command);

        if (0 == strcmp(command, "allocate"))
        {
            scanf("%d", &numBytesWanted);
            allocate(numBytesWanted, mHead);
            continue;
        }
        else if (0 == strcmp(command, "free"))
        {
            scanf("%d", &blockNumToDelete);
            freeheap(blockNumToDelete, mHead);
            continue;
        }
        else if (0 == strcmp(command, "blocklist"))
        {
            blocklist(mHead);
            continue;
        }
        else if (0 == strcmp(command, "writeheap"))
        {
            scanf("%d %c %d", &blockNumToWriteInto, &charToWrite, &numChars);
            writeheap(blockNumToWriteInto, charToWrite, numChars, mHead);
            continue;
        }
        else if (0 == strcmp(command, "printheap"))
        {
            scanf("%d%d", &blockNumToPrint, &numBytesToPrint);
            printheap(blockNumToPrint, numBytesToPrint, mHead);
            continue;
        }
        else if (0 == strcmp(command, "quit"))
        {
            flag = 0;
        }
        else
        {
            printf("Command Not Recognized\n");
            continue;
        }
    }
    while(flag);

    free(mHead);
    return 0;
}

void allocate(int numBytesWanted, char *head)
{
    char *blkHead = head;
    int splitFlag = 0;
    int payload = 0;

    // First Fit
    int index = 0;
    while (*blkHead && index < MALLOC_SIZE)
    {
        int blkSize = *((int*)blkHead);
        char *allocPtr = (blkHead + 2*sizeof(int));
        // Block is allocated
        if (*allocPtr == 'y')
        {
            blkHead += blkSize + HEADER_SIZE;
            index += blkSize + HEADER_SIZE;
            continue;
        }
        // Block is not allocated
        else
        {
            // Block is too small
            if (numBytesWanted > blkSize)
            {
                blkHead += blkSize + HEADER_SIZE;
                index += blkSize + HEADER_SIZE;
                continue;
            }
            // Block is large enough
            else
            {
                if (numBytesWanted == blkSize)
                    break;
                // split the block
                else {

                    // SPLIT
                    if (blkSize - numBytesWanted > HEADER_SIZE) {
                        payload = blkSize - numBytesWanted - HEADER_SIZE;
                        splitFlag = 1;
                    }
                    // Not large enough
                    else {
                        printf("Block size increased to %d.\n", blkSize);
                        numBytesWanted = blkSize;
                    }
                }
                break;
            }
        }
    }
    if (index >= MALLOC_SIZE) {
        printf("Insufficient Space\n");
        return;
    }

    printf("%d\n", blockID);

    char *blkPtr = blkHead;

    *((int*)blkPtr) = numBytesWanted;
    blkPtr += sizeof(int);
    *((int*)blkPtr) = blockID++;
    blkPtr += sizeof(int);
    *(blkPtr++) = 'y';

    int i;
    for(i = 0; i < numBytesWanted; ++i)
    {
        *blkPtr = ' ';
        blkPtr++;
    }

    if (splitFlag) {
        *((int*)blkPtr) = payload;
        blkPtr += sizeof(int);
        // *((int*)blkPtr) = blockID++;
        *((int*)blkPtr) = 0;
        blkPtr += sizeof(int);
        *(blkPtr++) = 'n';
    }
}

void freeheap(int blockNumToDelete, char *head)
{
    char *blkHead = head;

    int index = 0;
    while (*blkHead && index < MALLOC_SIZE)
    {

        // find id
        int blkSize = *((int*)blkHead);
        int id = *((int*)(blkHead + sizeof(int)));
        char *allocPtr = (blkHead + 2*sizeof(int));

        if (id == blockNumToDelete)
        {
            *allocPtr = 'n';
            return;
        }
        else
        {
            blkHead += blkSize + HEADER_SIZE;
            index += blkSize + HEADER_SIZE;
            continue;
        }
    }
    printf("Block ID Not Found\n");
}

void blocklist(char *head)
{
    printf("Size\tAllocated\tStart\t\tEnd\n");
    while (*head)
    {
        int size = *((int*)head);
        int start = head;
        int end = head + size + HEADER_SIZE - 1;
        char allocated = *(head + 2*sizeof(int));

        if (allocated == 'y')
            printf("%d\tyes\t\t0x%x\t0x%x\n", size, start, end);
        else
            printf("%d\tno\t\t0x%x\t0x%x\n", size, start, end);
        head = end + 1;
    }
}

void writeheap(int blockNumToWriteInto, char charToWrite, int numChars, char *head)
{
    char *blkHead = head;

    int index = 0;
    while (*blkHead && index < MALLOC_SIZE)
    {
        // find id
        int blkSize = *((int*)blkHead);
        int id = *((int*)(blkHead + sizeof(int)));
        char *allocPtr = (blkHead + 2*sizeof(int));

        if (id == blockNumToWriteInto)
        {
            if (*allocPtr != 'y') {
                printf("Invalid Block ID\n");
                return;
            }
            else if (blkSize < numChars) {
                printf("Block Not Big Enough\n");
                return;
            }

            blkHead += HEADER_SIZE;
            //write to block
            int i;
            for(i = 0; i < numChars; ++i)
            {
                *blkHead = charToWrite;
                blkHead++;
            }
            return;
        }
        else
        {
            blkHead += blkSize + HEADER_SIZE;
            index += blkSize + HEADER_SIZE;
            continue;
        }
    }
    printf("Block ID Not Found\n");
}

void printheap(int blockNumToPrint, int numBytesToPrint, char *head)
{
    char *blkHead = head;

    int index = 0;
    while (*blkHead && index < MALLOC_SIZE)
    {
        // find id
        int blkSize = *((int*)blkHead);
        int id = *((int*)(blkHead + sizeof(int)));
        char *allocPtr = (blkHead + 2*sizeof(int));

        if (id == blockNumToPrint)
        {
            if (*allocPtr != 'y') {
                printf("Invalid Block ID\n");
                return;
            }
            else if (blkSize < numBytesToPrint) {
                printf("Block Only %d Bytes\n", blkSize);
                numBytesToPrint = blkSize;
            }

            blkHead += HEADER_SIZE;
            //write to block
            int i;
            for(i = 0; i < numBytesToPrint; ++i)
            {
                printf("%c", *blkHead);
                blkHead++;
            }
            printf("\n");
            return;
        }
        else
        {
            blkHead += blkSize + HEADER_SIZE;
            index += blkSize + HEADER_SIZE;
            continue;
        }
    }
    printf("Block ID Not Found\n");
}

