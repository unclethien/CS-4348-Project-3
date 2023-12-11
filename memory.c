#include <stdio.h>
#include <stdlib.h>
#include "registers.h"

#define MemorySize 1024
#define WordSize 2

int memory[MemorySize][WordSize];

extern void deallocate(int);
extern int is_allowed_address(int, int);
extern void remove_process(int);

int *mem_read(int addr, int pid) 
{
    if(addr < 0 || addr > MemorySize) 
    {
        printf("Error: Invalid memory address: %d\n", addr);
        return NULL;
    }

    if (is_allowed_address(addr, pid)) 
    {
        return memory[addr];
    } 
    else 
    {
        deallocate(pid);
        remove_process(pid);
        return NULL;
    }
}

void mem_write(int addr, int* data, int pid) 
{
    if(addr < 0 || addr > MemorySize) 
    {
        printf("Error: Invalid memory address: %d\n", addr);
        return;
    }

    if (is_allowed_address(addr, pid)) {
        memory[addr][0] = data[0];
        memory[addr][1] = data[1];
    } else {
        deallocate(pid);
    }
}

void mem_load(char* fname)
{
    //Open file
    FILE *fp = fopen(fname, "r");
    if(fp == NULL)
    {
        printf("Error: Could not open file %s\n", fname);
        return;
    }
    //read instructions
    char line[64];
    while(fgets(line, 64, fp) != NULL)
    {
        printf("%s", line);
    }
    fclose(fp);

}
void mem_print()
{
    int i;
    for(i=0; i<20; i++)
    {
        printf("Addr: %d, Data: %d %d\n", i, memory[i][0], memory[i][1]); 
    }
}

void mem_init()
{
    int i, j;
    for(i=0; i<MemorySize; i++)
    {
        for(j=0; j<WordSize; j++)
        {
            memory[i][j] = 0;
        }

    }
}