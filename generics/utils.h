#include "../include/types_bikaya.h"

void initNewArea(memaddr handler, memaddr RRF_addr);
void setStatusReg(memaddr *statusReg, process_option *option);
void setPC(state_t *process, memaddr function);
void setStackP(state_t *process, memaddr memLocation);