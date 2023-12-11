#include <stdio.h>
#include <stdlib.h>


typedef struct MemorySegment 
{
    int base_address;
    int size;
    struct MemorySegment* next;
} MemorySegment;


MemorySegment* memory_holes = NULL;
int allocation_table[256][3];
int allocation_count = 0;



int find_hole(int size);
void add_hole(int base, int size);
void remove_hole(int base);
void merge_holes();
int find_empty_row();

int allocate(int pid, int size) 
{
    int base_address = find_hole(size);
    int row = find_empty_row();

    if (base_address == -1) 
    {
        return 0;
    } 
    else 
    {
        if (row == -1) 
        {
            allocation_count++;
        }
        else
        {
            row = allocation_count;
        }
        allocation_table[row][0] = pid;
        allocation_table[row][1] = base_address;
        allocation_table[row][2] = size;
        return 1;
    }
}

void deallocate(int pid) {
    int i;

    for (i = 0; i < allocation_count; i++) 
    {
        if (allocation_table[i][0] == pid) 
        {
            int base_address = allocation_table[i][1];
            int size = allocation_table[i][2];

            add_hole(base_address, size);
            allocation_table[i][2] = 0;

            break;
        }
    }
}

void add_hole(int base, int size) 
{
    MemorySegment* new_hole = (MemorySegment*)malloc(sizeof(MemorySegment));
    if (new_hole == NULL) 
    {
        printf("Error: Could not allocate memory for new hole\n");
        exit(1);
    }
    new_hole->base_address = base;
    new_hole->size = size;
    new_hole->next = NULL;

    if (memory_holes == NULL) 
    {
        memory_holes = new_hole;
    } 
    else 
    {
        MemorySegment* current = memory_holes;
        MemorySegment* previous = NULL;

        while (current != NULL && current->base_address < base) 
        {
            previous = current;
            current = current->next;
        }

        if (previous == NULL)
        {
            new_hole->next = memory_holes;
            memory_holes = new_hole;
        } 
        else 
        {
            previous->next = new_hole; 
            new_hole->next = current;
        }
    }

    merge_holes();
}

void remove_hole(int base) 
{
    MemorySegment* current = memory_holes;
    MemorySegment* previous = NULL;

    while (current != NULL && current->base_address != base) 
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL) 
    {
        printf("Error: Hole not found\n");
        exit(1);
    }

    if (previous == NULL) 
    {
        memory_holes = current->next;
    } else 
    {
        previous->next = current->next;
    }

    free(current);
}

void merge_holes() 
{
    MemorySegment* current = memory_holes;
    MemorySegment* next = current->next;

    while (next != NULL) 
    {
        if (current->base_address + current->size == next->base_address) 
        {
            current->size += next->size;
            current->next = next->next;
            free(next);
            next = current->next;
        } 
        else 
        {
            current = next;
            next = next->next;
        }
    }
}

int find_hole(int size) 
{
    if (memory_holes == NULL) 
    {
        memory_holes = (MemorySegment*)malloc(sizeof(MemorySegment));
        memory_holes -> size = 1024;
    }

    MemorySegment* current = memory_holes;
    MemorySegment* next = current->next;

    while (current != NULL) 
    {
        if (current->size >= size) 
        {
            int base_address = current->base_address;
            current->base_address += size;
            current->size -= size;

            if (current->size == 0) 
            {
                remove_hole(base_address);
            }

            return base_address;
        }

        current = current->next;
    }

    return -1;
}

int get_base_address(int pid) 
{
    int i;

    for (i = 0; i < allocation_count; i++) 
    {
        if (allocation_table[i][0] == pid) 
        {
            return allocation_table[i][1];
        }
    }

    return -1;
}

int find_empty_row() 
{
    int i;

    for (i = 0; i < allocation_count; i++) 
    {
        if (allocation_table[i][2] == 0) 
        {
            return i;
        }
    }

    return -1;
}

int is_allowed_address(int pid, int addr) 
{
    int i;

    for (i = 0; i < allocation_count; i++) 
    {
        if (allocation_table[i][0] == pid) 
        {
            int base_address = allocation_table[i][1];
            int size = allocation_table[i][2];

            if (addr >= base_address && addr < base_address + size) 
            {
                return 1;
            }
        }
    }

    return 0;
}

