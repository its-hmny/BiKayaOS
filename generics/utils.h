#ifndef __UTILS_H
#define __UTILS_H

#include "../include/types_bikaya.h"

// Matrix of current IO request/executions (the terminal is counted twice for receiving an transmission)
// This means that blocked[n-1] is transmission request IO instead blocked[n] is receive request
int IO_blocked[MULTIPLE_DEV_LINE + 1][DEV_PER_INT];

void wipe_Memory(void *memaddr, unsigned int size);
void initNewArea(memaddr handler, memaddr RRF_addr);
void setStatusReg(state_t *proc_state, process_option *option);
void setPC(state_t *process, memaddr function);
void setStackP(state_t *process, memaddr memLocation);
unsigned int getExCode(state_t *oldArea);
void cloneState(state_t *process_state, state_t *old_area, unsigned int size);
void init_time(time_t *process_time);
void update_time(unsigned int option, unsigned int current_time);
void loadCustomHandler(unsigned int exc_code);

#endif