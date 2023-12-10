#include <stdio.h>
#include "registers.h"

extern int registers[7];

extern int* mem_read(int);
extern void mem_write(int, int*);

int data[2];


struct register_struct context_switch(struct register_struct new_vals) {
    struct register_struct old_process = process;
    process = new_vals;
    return old_process;


    // register_struct old_vals = {
    //     .AC = registers[AC],
    //     .IR0 = registers[IR0],
    //     .IR1 = registers[IR1],
    //     .MBR = registers[MBR],
    //     .MAR = registers[MAR],
    //     .PC = registers[PC],
    //     .Base = registers[Base]
    // };

    // registers[AC] = new_vals.AC;
    // registers[IR0] = new_vals.IR0;
    // registers[IR1] = new_vals.IR1;
    // registers[MBR] = new_vals.MBR;
    // registers[MAR] = new_vals.MAR;
    // registers[PC] = new_vals.PC;
    // registers[Base] = new_vals.Base;

    // return old_vals;
}

void fetch_instruction(int addr)
{
    int* instruct = mem_read(addr);
    process.registers[IR0] = instruct[0];
    process.registers[IR1] = instruct[1];
}

void execute_instruction()
{
    int data[2];
    data[0] = process.registers[MBR];
    switch(process.registers[IR0])
    {
        case 0: //exit
            break;
        case 1: //load_const
            process.registers[AC] = process.registers[IR1];
            break;
        case 2: //move_from_mbr
            process.registers[AC] = process.registers[MBR];
            break;
        case 3: //move_from_mar
            process.registers[AC] = process.registers[MAR];
            break;
        case 4: //move_to_mbr
            process.registers[MBR] = process.registers[AC];
            break; 
        case 5: //move_to_mar
            process.registers[MAR] = process.registers[AC];
            break;
        case 6: //load_at_addr
            process.registers[MBR] = mem_read(process.registers[MAR])[0];
            break;
        case 7: //write_at_addr
            data[0] = 0;
            data[1] = process.registers[MBR];
            mem_write(process.registers[MAR], data);
            break;
        case 8: //add
            process.registers[AC] += process.registers[MBR];
            break;
        case 9: //multiply
            process.registers[AC] *= process.registers[MBR];
            break;
        case 10: //and
            if (process.registers[MBR] != 0 && process.registers[AC] != 0)
            {
                process.registers[AC] = 1;
            }
            else
            {
                process.registers[AC] = 0;
            }
            process.registers[MBR] = process.registers[MBR] && AC;
            break;
        case 11: //or
            if (process.registers[MBR] != 0 || process.registers[AC] != 0)
            {
                process.registers[AC] = 1;
            }
            else
            {
                process.registers[AC] = 0;
            }
            break;
        case 12: //ifgo
            
            if(process.registers[AC] != 0)
            {
                process.registers[PC] = process.registers[IR1] - 1;
            }
            break;
        case 13: //sleep
            break;
        default:
            printf("Invalid opcode: %d\n", IR0);
            break;
        
    }
}

int mem_address(int l_addr)
{
    return process.registers[Base] + l_addr;
}

int clock_cycle()
{
    fetch_instruction(mem_address(process.registers[PC]));
    execute_instruction();
    process.registers[PC]++;
    if(process.registers[IR0]==0)
    {
        return 0;
    }
    return 1;
}
