#ifndef REGISTERS_H
#define REGISTERS_H

struct register_struct {
    int registers[7];   
    enum {Base,PC,IR0,IR1,AC,MAR,MBR};
};

struct PCB {
    int pid;
    int size;
    int base;
    struct register_struct registers;
    
};

struct Node {
    struct PCB pcb;
    struct Node* next;
};

struct register_struct process;

#endif
