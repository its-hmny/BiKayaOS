#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "./utils.h"

HIDDEN void wipe_Memory(void *memaddr, unsigned int size) {
    unsigned char* tmp_p = memaddr;
    
    while(size--)
        *tmp_p++ = (unsigned char) 0;
}

void initNewArea(memaddr handler, memaddr RRF_addr) {
    state_t *newArea = (state_t*) RRF_addr; // Set the location of the New Area to the handler
    wipe_Memory(newArea, sizeof(state_t));

    // Set the state of the handler with disabled interrupt, kernel mode and so on (status register)
    process_option execpt_handler_option = { DISABLE_INTERRUPT, KERNEL_MD_ON, 0, VIRT_MEM_OFF, TIMER_DISABLED };
    setStatusReg(newArea, &execpt_handler_option);
    
    setPC(newArea, handler);
    
    setStackP(newArea, (memaddr)_RAMTOP);
}

/*unsigned int interruptEnabled;
    unsigned int kernelMode;
    unsigned int interrupt_mask;
    unsigned int virtualMemory;
    unsigned int timerEnabled;*/

#ifdef TARGET_UMPS
void setStatusReg(state_t *proc_state, process_option *option) {
    STATUS_REG(proc_state) |= option->interruptEnabled;
    STATUS_REG(proc_state) |= (option->kernelMode << KM_SHIFT);
    STATUS_REG(proc_state) |= (option->interrupt_mask << INTERRUPT_MASK_SHIFT);
    STATUS_REG(proc_state) |= (option->virtualMemory << VIRT_MEM_SHIFT);
    STATUS_REG(proc_state) |= (option->timerEnabled << TIMER_SHIFT);
}
#endif

#ifdef TARGET_UARM
void setStatusReg(state_t *proc_state, process_option *option) {
    STATUS_REG(proc_state) = (option->kernelMode) ? (STATUS_SYS_MODE) : (STATUS_USER_MODE); 
    STATUS_REG(proc_state) = (option->interruptEnabled) ? (STATUS_ENABLE_INT(STATUS_REG(proc_state))) : (STATUS_DISABLE_INT(STATUS_REG(proc_state)));
    proc_state->CP15_Control = (option->virtualMemory) ? (CP15_ENABLE_VM(proc_state->CP15_Control)) : (CP15_DISABLE_VM(proc_state->CP15_Control));
    STATUS_REG(proc_state) = (option->timerEnabled) ? (STATUS_ENABLE_TIMER(STATUS_REG(proc_state))) : (STATUS_DISABLE_TIMER(STATUS_REG(proc_state)));
    
    /*
	s->cpsr = STATUS_SYS_MODE;
	s->cpsr = STATUS_DISABLE_INT(s->cpsr);
	s->CP15_Control = CP15_DISABLE_VM(s->CP15_Control);
    */

    /*int status = STATUS_REG(proc_state);
    status = STATUS_DISABLE_INT(status) | STATUS_ENABLE_TIMER(status) | STATUS_SYS_MODE;*/
}
#endif

void setPC(state_t *process, memaddr function) {
    PC_REG(process) = function;
}

void setStackP(state_t *process, memaddr memLocation) {
    SP_REG(process) = memLocation;
}