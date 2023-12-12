
#include <stdio.h>
#include "registers.h"

extern void load_programs(char[]);
extern void load_prog(char*, int);
extern int clock_cycle();
extern void mem_print();
extern void mem_init();

extern int get_current_process_pid();
int schedule(int, int );

int main(int argc, char *argv[])
{
    // Check command line arguments
    if(argc < 2) 
    {
        printf("Usage: ./main programfile\n");
        return 1;
    }

    char* prog_file = argv[1];
    
    // Initialize memory
    mem_init();
    
    // Load all programs
    load_programs(prog_file);
    
    int cycle_count = 0;
    int run = 1;
    int hole_count = 0; // Variable to count the number of holes created
    
    while(schedule(cycle_count, clock_cycle()) && run)
    {
        cycle_count++;
        
        if(cycle_count > 10){
            cycle_count = 1;
        }
        
    }
    
    // Print final memory contents
    mem_print();
    
    // Print the number of holes created
    printf("Number of holes created: %d\n", hole_count);
    
    return 0;
}
