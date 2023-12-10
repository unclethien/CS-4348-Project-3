
#include <stdio.h>
#include "registers.h"

extern void load_programs(char[]);
extern void load_prog(char*, int);
extern int clock_cycle();
extern void mem_print();
extern void mem_init();
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
        
        // Check if there are any processes left in the ready queue
        if (/* condition to check if ready queue is empty */) {
            run = 0;
        }
        
        // Count the number of holes created during the runtime of the OS
        if (/* condition to check if a hole is created */) {
            hole_count++;
        }
    }
    
    // Print final memory contents
    mem_print();
    
    // Print the number of holes created
    printf("Number of holes created: %d\n", hole_count);
    
    // Print the specified memory locations
    printf("Memory locations: { 30, 150, 230 }\n");
    printf("Location 30: %d\n", /* value at location 30 */);
    printf("Location 150: %d\n", /* value at location 150 */);
    printf("Location 230: %d\n", /* value at location 230 */);
    
    return 0;
}
