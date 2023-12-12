#include <stdio.h>
#include <stdlib.h>
#include "registers.h"

#define MAX_PROCESSES 1024
#define TIME_QUANTUM 10


struct Node* ready_queue = NULL;
struct PCB process_table[MAX_PROCESSES];
int current_process_index = -1;
extern struct register_struct context_switch(struct register_struct new_vals);

void new_process(int base, int size, int pid) 
{
    struct PCB pcb;
    pcb.pid = pid;
    pcb.size = size;
    pcb.base = base;
    pcb.registers.registers[Base] = base;
    pcb.registers.registers[PC] = 0;

    process_table[pid] = pcb;

    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->pcb = pcb;
    new_node->next = NULL;

    if (ready_queue == NULL) {
        ready_queue = new_node;
        context_switch(pcb.registers);
    } else {
        struct Node* current = ready_queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void remove_process(int pid) 
{
    struct Node* current = ready_queue;
    struct Node* previous = NULL;

    while (current != NULL) {
        if (current->pcb.pid == pid) 
        {
            if (previous == NULL)  // first node
            {
                ready_queue = current->next;
                context_switch(ready_queue->pcb.registers);
            } 
            else 
            {
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

int get_current_process_pid() 
{
    struct Node* current = ready_queue;
    while(current){
        current = current->next;
    }
    printf("\n");
    if (ready_queue != NULL) 
    {
        return ready_queue->pcb.pid;
    }
    return -1;
}

void next_process() 
{
    struct Node* current = ready_queue;
    ready_queue = ready_queue->next;
    current->next = NULL;
    if (ready_queue == NULL) 
    {
        ready_queue = current;
    } 
    else 
    {
        struct Node* temp = ready_queue;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = current;
    }
}

int schedule(int cycle_num, int process_status) 
{
    if (ready_queue == NULL) 
    { 
        return 0;
    }
    if (cycle_num == 10) 
    {
        next_process();

        struct Node* temp = ready_queue;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }

        temp->pcb.registers = context_switch(ready_queue->pcb.registers);
    }

    if (process_status == 0) {
        struct Node* current = ready_queue;
        if (current->next == NULL) 
        {
            // free(ready_queue);
            return 0;
        }
        else
        {
            ready_queue = ready_queue->next;
            // free(current);
            context_switch(ready_queue->pcb.registers); 
        }
    }
    return 1;
}
