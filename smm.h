#ifndef SMM_H
#define SMM_H

// Declaration of the memory management functions
void init_smm();
int allocate(int requested_size);
void deallocate(int base_address);
int check_access(int pid, int address);

#endif
