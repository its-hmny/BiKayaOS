#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "./utils.h"

HIDDEN void wipe_Memory(void *memaddr, unsigned int size) {
    unsigned char* tmp_p = memaddr;
    
    while(size--)
        *tmp_p++ = (unsigned char) 0;
}

void initNewArea(memaddr handler, memaddr RRF_addr) {
    state_t *newArea = RRF_addr; // Set the location of the New Area to the handler
    wipe_Memory(newArea, sizeof(state_t));

    // Set the state of the handler with disabled interrupt, kernel mode and so on (status register)
    process_option execpt_handler_option = { DISABLE_INTERRUPT, KERNEL_MD_ON, 0, 0, VIRT_MEM_OFF, 0, TIMER_DISABLED };
    setStatusReg(newArea, &execpt_handler_option);
    
    setPC(newArea, handler);
    
    setStackP(newArea, (memaddr)RAMTOP);
}

// Manca la parte per uARM
void setStatusReg(state_t *proc_state, process_option *option) {
    /*STATUS_REG(proc_state) |= option->interruptEnabled;
    STATUS_REG(proc_state) |= (option->kernelMode << KM_SHIFT);
    STATUS_REG(proc_state) |= (option->PO_mask << PO_MASK_SHIFT);
    STATUS_REG(proc_state) |= (option->interrupt_mask << INTERRUPT_MASK_SHIFT);
    STATUS_REG(proc_state) |= (option->virtualMemory << VIRT_MEM_SHIFT);
    STATUS_REG(proc_state) |= (option->PO_VirtMem << VIRTMEM_PO_SHIFT);
    STATUS_REG(proc_state) |= (option->timerEnabled << TIMER_SHIFT);*/
}

void setPC(state_t *process, memaddr function) {
    PC_REG(process) = function;
}

void setStackP(state_t *process, memaddr memLocation) {
    SP_REG(process) = memLocation;
}