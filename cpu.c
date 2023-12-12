#include <stdio.h>
#include "registers.h"

extern int registers[7];

extern int* mem_read(int, int);
extern void mem_write(int, int*, int);

extern int get_current_process_pid();


int data[2];


struct register_struct context_switch(struct register_struct new_vals) {
    struct register_struct old_process = process;
    process = new_vals;
    return old_process;
}

void fetch_instruction(int addr)
{
    int* instruct = mem_read(addr, get_current_process_pid());
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
            process.registers[MBR] = mem_read(process.registers[MAR], get_current_process_pid())[0];
            break;
        case 7: //write_at_addr
            data[0] = 0;
            data[1] = process.registers[MBR];
            mem_write(process.registers[MAR], data, get_current_process_pid());
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
