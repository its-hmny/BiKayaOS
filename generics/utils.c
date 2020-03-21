#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "./utils.h"

/*
Prese dal sorgente per facilitare QUESTO E PER UMPS

#define STATE_GPR_LEN 29

typedef struct state {
    unsigned int entry_hi;  This registrer contains contains the current ASID
    unsigned int cause;  Cause of the exception that blocked this process
    unsigned int status; Status of the processor
    unsigned int pc_epc; Old/New area program counter, this is were the PC has to be saved
    unsigned int gpr[STATE_GPR_LEN]; Auxiliary registrer of the CPU (Ramtop and kernel reg excluded)
    unsigned int hi; 
    unsigned int lo; This two registrer insted are used for intermediate values in mutiplication and division
} state_t;
*/

void initNewArea(memaddr handler, memaddr RRF_addr) {
    state_t *newArea = RRF_addr; // Set the location of the New Area to the handler

    // Set the state of the handler with disabled interrupt, kernel mode and so on (status register)
    process_option execpt_handler_option = { DISABLE_INTERRUPT, KERNEL_MD_ON, 0, 0, VIRT_MEM_OFF, 0, TIMER_DISABLED };
    setStatusReg(&newArea->status, &execpt_handler_option);

    // Set the other register
    newArea->entry_hi = 0;
    newArea->cause = 0;
    
    //Clean the GPR, HI and LO register
    for (int i = 0; i <= GPR_LENGTH; i++)
        newArea->gpr[i] = 0;

    setPC(newArea, handler);
    setStackP(newArea, (memaddr)RAMTOP);
}

void setStatusReg(memaddr *statusReg, process_option *option) {
    *statusReg |= option->interruptEnabled;
    *statusReg |= (option->kernelMode << KM_SHIFT);
    *statusReg |= (option->PO_mask << PO_MASK_SHIFT);
    *statusReg |= (option->interrupt_mask << INTERRUPT_MASK_SHIFT);
    *statusReg |= (option->virtualMemory << VIRT_MEM_SHIFT);
    *statusReg |= (option->PO_VirtMem << VIRTMEM_PO_SHIFT);
    *statusReg |= (option->timerEnabled << TIMER_SHIFT);
}

void setPC(state_t *process, memaddr function) {
    process->pc_epc = function;
}

void setStackP(state_t *process, memaddr memLocation) {
    process->reg_sp = memLocation;
}