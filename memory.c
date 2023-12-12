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

    if (is_allowed_address(pid, addr)) 
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

void mem_write(int addr, int firstData, int secondData, int pid) 
{
    if(addr < 0 || addr > MemorySize) 
    {
        printf("Error: Invalid memory address: %d\n", addr);
        return;
    }

    if (is_allowed_address(pid, addr)) {
        memory[addr][0] = firstData;
        memory[addr][1] = secondData;
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
    printf("Addr: %d, Data: %d %d\n", 30, memory[30][0], memory[30][1]); 
    printf("Addr: %d, Data: %d %d\n", 150, memory[150][0], memory[150][1]); 
    printf("Addr: %d, Data: %d %d\n", 230, memory[230][0], memory[230][1]); 

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