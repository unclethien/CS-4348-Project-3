#include <stdio.h>
#include <stdlib.h>

int allocate(int pid, int size) {
    int base_address = find_hole(size);

    if (base_address == -1) {
        return 0;
    } else {
        AllocationEntry entry;
        entry.pid = pid;
        entry.size = size;
        entry.base_address = base_address;
        allocation_table[allocation_count++] = entry;
        return 1;
    }
}

void deallocate(int pid) {
    int i;

    for (i = 0; i < allocation_count; i++) {
        if (allocation_table[i].pid == pid) {
            int base_address = allocation_table[i].base_address;
            int size = allocation_table[i].size;

            add_hole(base_address, size);
            allocation_table[i].size = 0;

            break;
        }
    }
}

void add_hole(int base, int size) {
    MemorySegment* new_hole = (MemorySegment*)malloc(sizeof(MemorySegment));
    if (new_hole == NULL) {
        printf("Error: Could not allocate memory for new hole\n");
        exit(1);
    }
    new_hole->base_address = base;
    new_hole->size = size;
    new_hole->next = NULL;

    if (memory_holes == NULL) {
        memory_holes = new_hole;
    } else {
        MemorySegment* current = memory_holes;
        MemorySegment* previous = NULL;

        while (current != NULL && current->base_address < base) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            new_hole->next = memory_holes;
            memory_holes = new_hole;
        } else {
            previous->next = new_hole;
            new_hole->next = current;
        }
    }

    merge_holes();
}

void remove_hole(int base) {
    MemorySegment* current = memory_holes;
    MemorySegment* previous = NULL;

    while (current != NULL && current->base_address != base) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: Hole not found\n");
        exit(1);
    }

    if (previous == NULL) {
        memory_holes = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
}

void merge_holes() {
    MemorySegment* current = memory_holes;
    MemorySegment* next = current->next;

    while (next != NULL) {
        if (current->base_address + current->size == next->base_address) {
            current->size += next->size;
            current->next = next->next;
            free(next);
            next = current->next;
        } else {
            current = next;
            next = next->next;
        }
    }
}

int find_hole(int size) {
    MemorySegment* current = memory_holes;

    while (current != NULL) {
        if (current->size >= size) {
            int base_address = current->base_address;
            current->base_address += size;
            current->size -= size;

            if (current->size == 0) {
                remove_hole(base_address);
            }

            return base_address;
        }

        current = current->next;
    }

    return -1;
}

int get_base_address(int pid) {
    int i;

    for (i = 0; i < allocation_count; i++) {
        if (allocation_table[i].pid == pid) {
            return allocation_table[i].base_address;
        }
    }

    return -1;
}

int find_empty_row() {
    int i;

    for (i = 0; i < allocation_count; i++) {
        if (allocation_table[i].size == 0) {
            return i;
        }
    }

    return -1;
}

int is_allowed_address(int pid, int addr) {
    int i;

    for (i = 0; i < allocation_count; i++) {
        if (allocation_table[i].pid == pid) {
            int base_address = allocation_table[i].base_address;
            int size = allocation_table[i].size;

            if (addr >= base_address && addr < base_address + size) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    return 0;
}
