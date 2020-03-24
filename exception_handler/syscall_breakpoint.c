#include "../include/system_const.h"
#include "../process/scheduler.h"
#include "../process/asl.h"
#include "../process/pcb.h"
#include "syscall_breakpoint.h"

/*===================TODO REMOVE=========*/
#define ST_READY         1
#define ST_BUSY          3
#define ST_TRANSMITTED   5
#define CMD_ACK          1
#define CMD_TRANSMIT     2
#define CHAR_OFFSET      8
#define TERM_STATUS_MASK 0xFF

#ifdef TARGET_UMPS
static termreg_t *term0_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, 0);

static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

void termprint_tmp(char *str) {
    while (*str) {
        unsigned int stat = tx_status(term0_reg);
        if (stat != ST_READY && stat != ST_TRANSMITTED)
            return;

        term0_reg->transm_command = (((*str) << CHAR_OFFSET) | CMD_TRANSMIT);

        while ((stat = tx_status(term0_reg)) == ST_BUSY)
            ;

        term0_reg->transm_command = CMD_ACK;

        if (stat != ST_TRANSMITTED)
            return;
        else
            str++;
    }
}
#endif
#ifdef TARGET_UARM
#define termprint_tmp(str) tprint(str);
#endif

/* ================ SYSCALL DEFINITION ================ */

HIDDEN void syscall3(pcb_t *root) {
    if (root == NULL)
        return;

    // Removes the root from father's child list
    outChild(root);
    // Removes it from the semaphor's queue, if present
    outBlocked(root);
    // Removes it from the ready queue, if present
    outProcQ(getReadyQ(), root);

    struct list_head *tmp = NULL;

    list_for_each(tmp, &root->p_child)
        syscall3(container_of(tmp, pcb_t, p_sib));
}




/* ========== SYSCALL & BREAKPOINT HANDLER ========== */

// A pointer to the old area, used to retrieve info about the exception
state_t *old_area = NULL;


// Returns the exception code from the cause registrer in the old area
HIDDEN unsigned int getExCode(void) {
    unsigned int causeReg = CAUSE_REG(old_area);
    return(CAUSE_GET_EXCCODE(causeReg));
}


/* 
    This function takes the syscall number and call the appropriate system call

    sysNumber: the syscall number retrieved from the Old Area
    return: void
*/
HIDDEN void syscallDispatcher(unsigned int sysNumber) {
    switch (sysNumber) {
        case 1:
            termprint_tmp("Syscall 1 \n");
            PANIC();
            break;

        case 2:
            termprint_tmp("Syscall 2 \n");
            PANIC();
            break;

        case 3:
            // Kill the current process wich has called the syscall
            syscall3(getCurrentProc());
            // Calls the scheduer to execute a new process
            scheduler();
            break; 

        case 4:
            termprint_tmp("Syscall 4 \n");
            PANIC();
            break;

        case 5:
            termprint_tmp("Syscall 5 \n");
            PANIC();
            break;

        case 6:
            termprint_tmp("Syscall 6 \n");
            PANIC();
            break;

        case 7:
            termprint_tmp("Syscall 7 \n");
            PANIC();
            break;

        case 8:
            termprint_tmp("Syscall 8 \n");
            PANIC();
            break;

        case 9:
            termprint_tmp("Syscall 9 \n");
            PANIC();
            break;

        case 10:
            termprint_tmp("Syscall 10 \n");
            PANIC();
            break; 

        case 11:
            termprint_tmp("Syscall 11 \n");
            PANIC();
            break;

        default:
            termprint_tmp("Syscall code not recognised \n");
            PANIC();
            break;
    }
}


void syscall_breakpoint_Handler(void) {
    // Retrieve the old area, where the previous state is saved and extrapolate the exception code
    old_area = (state_t*) OLD_AREA_SYSCALL;
    unsigned int exCode = getExCode(); 

    // Checsks if the code is for a syscall and not a breakpoint
    if (exCode == SYSCALL_CODE) {  
        unsigned int numberOfSyscall = GET_A0_REG(old_area);
        syscallDispatcher(numberOfSyscall);
    }
    // Else is a breakpoint 
    else if(exCode == BREAKPOINT_CODE){
        termprint_tmp("Breakpoint reached! ERROR\n");
        PANIC();
    }
    else{
        termprint_tmp("OMG NON SO COSA CAZZO È, ORA VADO IN PANICO");
        PANIC();
    }
}