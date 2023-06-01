#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "cachelab.h"

//initialize counters and variables
int counter = 0;
int hits = 0;
int misses = 0;
int evictions = 0;
int verbose;
int opt;
int setbits = 0;
int associativity = 0;
int blockbits = 0;
int a = 0;

struct cache {
    int valid;
    int tag;
    int block; //maybe simplify to just int instead of array
    int timestamp;
};

//data load
// first, check if it matches and return a hit if it does technically we return the block data
// if the block is empty, we write the data into it and return a miss
// if the whole set is full, we replace the least recently used return a miss and evict
void dataload(char instruction, long int address, int size, struct cache **memory, 
    int setmask, int blockmask)
{
    int set = setmask&address;
    set = set>>blockbits;
    int bit = blockmask&address;
    long int tag = address>>(blockbits+setbits);
    long int LRU = 0xffffffffff;
    int x;
    int y;
    for (a = 0; a < associativity; a++)
    {
        if (memory[set][a].valid == 1 && memory[set][a].tag == tag) //match case
        {
            if (verbose == 1){
                printf("\n %c %lx %d data hit, match case", instruction, address, size);
            }
            hits++;
            counter = counter + 1;
            memory[set][a].timestamp = counter;
            return;
        }
        else if (memory[set][a].valid == 0) //empty case
        {
            if(verbose == 1){
                printf("\n %c %lx %d data miss, empty case", instruction, address, size);
            }
            misses++;
            memory[set][a].valid = 1;
            memory[set][a].tag = tag;
            memory[set][a].block = bit+size;
            counter = counter + 1;
            memory[set][a].timestamp = counter;
            return;
        }
        else //block is occupied and we iterate through to find LRU
        {
            if (memory[set][a].valid == 1 && memory[set][a].timestamp < LRU)
            {
                LRU = memory[set][a].timestamp;
                x = set;
                y = a;
            }
        }
    }
    if(verbose == 1){
        printf("\n %c %lx %d data miss, evict case", instruction, address, size);
    }
    misses++;
    evictions++;
    memory[x][y].valid = 1;
    memory[x][y].tag = tag;
    memory[x][y].block = bit+size;
    counter++;
    memory[x][y].timestamp = counter;
    return;
}

//data store
// first, check if it matches and return a hit if it does technically we write the block data
// if the block is empty, we write the data into it and return a miss
// if the whole set is full, we replace the least recently used return a miss and evict
void datastore(char instruction, long int address, int size, struct cache **memory, 
    int setmask, int blockmask)
{
    int set = setmask&address;
    set = set>>blockbits;
    int bit = blockmask&address;
    long int tag = address>>(blockbits+setbits);
    long int LRU = 100000000;
    int x;
    int y;
    for (a = 0; a < associativity; a++)
    {
        if (memory[set][a].valid == 1 && memory[set][a].tag == tag) //match case
        {
            if(verbose == 1){
                printf("\n %c %lx %d data hit, match case", instruction, address, size);
            }
            hits++;
            memory[set][a].block = bit+size;
            counter++;
            memory[set][a].timestamp = counter;
            return;
        }
        else if (memory[set][a].valid == 0) //empty case
        {
            if(verbose == 1){
                printf("\n %c %lx %d data miss, empty case", instruction, address, size);
            }
            misses++;
            memory[set][a].valid = 1;
            memory[set][a].tag = tag;
            memory[set][a].block = bit+size;
            counter++;
            memory[set][a].timestamp = counter;
            return;
        }
        else //block is occupied and we iterate through to find LRU
        {
            if (memory[set][a].valid == 1 && memory[set][a].timestamp < LRU)
            {
                LRU = memory[set][a].timestamp;
                x = set;
                y = a;
            }
        }
    }
    
    if(verbose == 1){
        printf("\n %c %lx %d data miss, evict case", instruction, address, size);
    }
    misses++;
    evictions++;
    memory[x][y].valid = 1;
    memory[x][y].tag = tag;
    memory[x][y].block = bit+size;
    counter++;
    memory[x][y].timestamp = counter;
    return;
}

int main(int argc, char *argv[])
{
    //initialize counters and variables
    char* file = NULL;

    //parse command line input
    while((opt=getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch(opt)
        {
            case 'h':
            case 'v': //optional but helpful verbose flag
                verbose = 1;
                break;
            case 's':
                setbits = atoi(optarg);
                break;
            case 'E':
                associativity = atoi(optarg);
                break;
            case 'b':
                blockbits = atoi(optarg);
                break;
            case 't':
                file = optarg;
                break;
        }
    }

    // setting up cache representation in memory
    // int blocksize = 2<<(blockbits - 1);
    int numsets = 2<<(setbits - 1);

    struct cache **memory = (struct cache **)malloc(sizeof(struct cache*) * numsets);
    for (int i = 0; i < numsets; i++)
    {
        memory[i] = (struct cache *)malloc(sizeof(struct cache) * associativity);
        for (int j = 0; j < associativity; j++)
        {
            memory[i][j].valid = 0;
            memory[i][j].timestamp = 0;
        }
    }

    // masks to load file inputs:
    int blockmask = (1<<blockbits) - 1; // makes bitmask of block bit # of 1s
    int setmask = (1<<setbits) - 1;
    setmask = setmask<<blockbits;
    // int tagmask = -1^setmask^blockmask;
    // alternative method to get tag bits is to use mask
    // simpler to just right shift tag by # of setbits + # of block bits

    //size instruction is not byte index but length of instruction
    // for example,  L 00602260,4 loads data at 2260-2263
    // convert to binary and add leading 0s if necessary to make address 64 bits
    // e.g. 10 becomes 0...000010000 with 59 (?) leading 0s

    //parse file input
    char * buffer = (char *)malloc(200);
    size_t bufsize = setbits + blockbits + 1000; //include large space for instruction commands
    size_t characters;
    FILE * restrict file_getline = fopen(file, "r"); //r indicates read operation

    //error case
    buffer = (char*)malloc(bufsize * sizeof(char));
    if (buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    
    //set up variables for loop
    int error; //variable to make sure there is no error
    char instruction;
    long int address;
    int size;

    //loop through file
    while ((characters = getline(&buffer, &bufsize, file_getline)) != -1)
    {
        error = sscanf(buffer, " %c %lx,%d", &instruction, &address, &size);
        if (error == -1)
        {
            printf("ERROR: sscanf error");
        }
        else if (instruction == 'L') //data load instruction
        {
            dataload(instruction, address, size, memory, setmask, blockmask);
        }
        else if (instruction == 'S') // data store instruction
        {
            datastore(instruction, address, size, memory, setmask, blockmask);
        }
        else if (instruction == 'M') // data load, then store
        {
            dataload(instruction, address, size, memory, setmask, blockmask);
            datastore(instruction, address, size, memory, setmask, blockmask);        
        }
    }

    printSummary(hits, misses, evictions); //need to include to be graded
    return 0;
}