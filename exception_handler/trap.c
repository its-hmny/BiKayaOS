#include "./trap.h"
#include "../include/system_const.h"
#include "./syscall_breakpoint.h"
#include "../include/types_bikaya.h"
#include "../process/scheduler.h"
#include "../generics/utils.h"

HIDDEN state_t *oldarea = NULL;

void trap_handler(void) {
    oldarea = (state_t*) OLD_AREA_TRAP;
    PC_REG(oldarea) += WORDSIZE;
    
    pcb_t* caller = getCurrentProc();
    
    unsigned int has_handler = caller->custom_hndlr.has_custom_handler[TRAP_CUSTOM];
    if (! has_handler) 
        syscallDispatcher(TERMINATEPROCESS);

    cloneState(caller->custom_hndlr.trap_old, oldarea, sizeof(state_t));
    LDST(caller->custom_hndlr.trap_new);
}