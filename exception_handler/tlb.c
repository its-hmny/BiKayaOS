#include "./tlb.h"
#include "../include/system_const.h"
#include "./syscall_breakpoint.h"

state_t *old_Area = NULL;

#define TLB_CUSTOM 2

void tlb_handler(void) {
    old_Area = (state_t*) OLD_AREA_TLB;
    PC_REG(old_Area) += WORDSIZE;
    
    pcb_t* caller = getCurrentProc();
    
    unsigned int has_handler = caller->custom_hndlr.has_custom_handler[TLB_CUSTOM];
    if (! has_handler) 
        syscallDispatcher(TERMINATEPROCESS);

    cloneState(caller->custom_hndlr.syscall_old, old_Area, sizeof(state_t));
    LDST(caller->custom_hndlr.tlb_new);
}