#ifndef __UTILS_H
#define __UTILS_H

#include "../include/types_bikaya.h"

void wipe_Memory(void *memaddr, unsigned int size);
void initNewArea(memaddr handler, memaddr RRF_addr);
void setStatusReg(state_t *proc_state, process_option *option);
void setPC(state_t *process, memaddr function);
void setStackP(state_t *process, memaddr memLocation);
unsigned int getExCode(state_t *oldArea);
void cloneState(state_t *process_state, state_t *old_area, unsigned int size);
void update_time(unsigned int *time_counter, unsigned int last_update, unsigned int current_time);

#endif