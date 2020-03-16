#include "../include/types_bikaya.h"

void initNewArea(memaddr handler, memaddr RRF_addr);
void setStatusReg(memaddr *statusReg, process_option *option);
inline void setPC(state_t *process, memaddr function);
inline void setStackP(state_t *process, memaddr memLocation);