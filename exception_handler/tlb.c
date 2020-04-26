#include "./tlb.h"
#include "../include/system_const.h"
#include "./syscall_breakpoint.h"
#include "../include/types_bikaya.h"
#include "../process/scheduler.h"
#include "../generics/utils.h"

HIDDEN state_t *old_area = NULL;

void tlb_handler(void) {
    old_area = (state_t*) OLD_AREA_TLB;
    
    pcb_t *caller = getCurrentProc();
    unsigned int has_handler = caller->custom_hndlr.has_custom_handler[TLB_CUSTOM];
    
    // In case a process doesn't have a custom handler, it's killed
    if (! has_handler) 
        terminate_process(caller);

    else {
        // Saves the state of the current process and loads the custom handler
        cloneState(caller->custom_hndlr.tlb_old, old_area, sizeof(state_t));
        LDST(caller->custom_hndlr.tlb_new);
    }
}