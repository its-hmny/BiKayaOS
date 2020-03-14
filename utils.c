#include "include/types_bikaya.h"
#include "include/const.h"
#include "include/system.h"

#define HIDDEN static

// Status registrer bits for enabling/disabling interrupts in the given process
#define DISABLE_INTERRUPT 0
#define ENABLE_INTERRUPT 1 
#define INTERRUPT_MASK_SHIFT 8

// Status registrer bits for enabling/disabling kernel mode in the given process
#define KERNEL_MD_ON 0
#define USR_MD_ON 1
#define KM_SHIFT 1

#define PO_MASK_SHIFT 2

#define VIRT_MEM_ON 1
#define VIRT_MEM_OFF 0
#define VIRT_MEM_SHIFT 24

#define VIRTMEM_PO_SHIFT 25

#define TIMER_ENABLED 1
#define TIMER_DISABLED 0
#define TIMER_SHIFT 27

// The last 30 word of the state_t struct, that are auxiliary registrers (intermediate values, etc)
#define GPR_LENGTH 30
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

#define reg_at  gpr[0]
#define reg_v0  gpr[1]
#define reg_v1  gpr[2]
#define reg_a0  gpr[3]
#define reg_a1  gpr[4]
#define reg_a2  gpr[5]
#define reg_a3  gpr[6]
#define reg_t0  gpr[7]
#define reg_t1  gpr[8]
#define reg_t2  gpr[9]
#define reg_t3  gpr[10]
#define reg_t4  gpr[11]
#define reg_t5  gpr[12]
#define reg_t6  gpr[13]
#define reg_t7  gpr[14]
#define reg_s0  gpr[15]
#define reg_s1  gpr[16]
#define reg_s2  gpr[17]
#define reg_s3  gpr[18]
#define reg_s4  gpr[19]
#define reg_s5  gpr[20]
#define reg_s6  gpr[21]
#define reg_s7  gpr[22]
#define reg_t8  gpr[23]
#define reg_t9  gpr[24]
#define reg_gp  gpr[25]
#define reg_sp  gpr[26]
#define reg_fp  gpr[27]
#define reg_ra  gpr[28]
#define reg_HI  gpr[29]
#define reg_LO  gpr[30]
*/

void initNewArea(memaddr handler, memaddr RRF_addr) {
    state_t *newArea = RRF_addr; // Set the location of the New Area to the handler

    // Set the state of the handler with disabled interrupt, kernel mode and so on (status register)
    process_option execpt_handler_option = { DISABLE_INTERRUPT, KERNEL_MD_ON, NULL, NULL, VIRT_MEM_OFF, NULL, TIMER_ENABLED };
    setStatusReg(&newArea->status, &execpt_handler_option);

    // Set the other register
    newArea->entry_hi = 0;
    newArea->cause = 0;
    
    //Clean the GPR, HI and LO register
    for (int i = 0; i <= GPR_LENGTH; i++)
        newArea->gpr[i] = 0;

    newArea->pc_epc = handler;
    newArea->reg_sp = RAMTOP;
}

void setStatusReg(unsigned int *statusReg, process_option *option) {
    *statusReg |= option->interruptEnabled;
    *statusReg |= (option->kernelMode << KM_SHIFT);
    *statusReg |= (option->PO_mask << PO_MASK_SHIFT);
    *statusReg |= (option->interrupt_mask << INTERRUPT_MASK_SHIFT);
    *statusReg |= (option->virtualMemory << VIRT_MEM_SHIFT);
    *statusReg |= (option->PO_VirtMem << VIRTMEM_PO_SHIFT);
    *statusReg |= (option->timerEnabled << TIMER_SHIFT);
}