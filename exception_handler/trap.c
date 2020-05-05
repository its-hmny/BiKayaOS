#include "./trap.h"
#include "../include/system_const.h"
#include "./syscall_breakpoint.h"
#include "../include/types_bikaya.h"
#include "../process/scheduler.h"
#include "../generics/utils.h"

HIDDEN state_t *old_area = NULL;

void trap_handler(void) {
    old_area = (state_t*) OLD_AREA_TRAP;
    
    // NOT SURE ABOUT THAT
    //PC_REG(oldarea) += WORDSIZE;

    loadCustomHandler(TRAP_CUSTOM, old_area);
}