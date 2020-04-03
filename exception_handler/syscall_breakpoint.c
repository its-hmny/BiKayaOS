#include "../include/system_const.h"
#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "../process/asl.h"
#include "../process/pcb.h"
#include "syscall_breakpoint.h"


// A pointer to the old area, used to retrieve info about the exception
state_t *old_area = NULL;

/* ================ SYSCALL DEFINITION ================ */

/*
    This syscall terminates the process given as input, removing recursively
    from the ASL, the ready queue and the father's child list.
    This is done for all the descendants of the given PCB (Sons, Grandsons, etc).

    root: a PCB pointer to the process to terminate
*/
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

    freePcb(root);
}



/* ========== SYSCALL & BREAKPOINT HANDLER ========== */

/* 
    This function takes the syscall number and call the appropriate system call,
    eventually initializing the args of each syscall. If a syscall is not recognized
    or implemented then issue a Kernel Panic

    sysNumber: the syscall number retrieved from the Old Area
    return: void
*/
HIDDEN void syscallDispatcher(unsigned int sysNumber) {
    switch (sysNumber) {
        case 1:
            PANIC();
            break;

        case 2:
            PANIC();
            break;

        case 3:
            // Kill the current process wich has called the syscall
            syscall3(getCurrentProc());
            // The current proc has been killed (dangling reference)
            setCurrentProc(NULL);
            // Calls the scheduer to execute a new process
            scheduler();
            break; 

        case 4:
            PANIC();
            break;

        case 5:
            PANIC();
            break;

        case 6:
            PANIC();
            break;

        case 7:
            PANIC();
            break;

        case 8:
            PANIC();
            break;

        case 9:
            PANIC();
            break;

        case 10:
            PANIC();
            break; 

        case 11:
            PANIC();
            break;

        default:
            PANIC();
            break;
    }
}

/*
    This is the handler of the syscall/breakpoint new area.
    It checks for the cause register exception code and eventually calls
    the subhandlers (one for syscall, one for breakpoiints).

    return: void
*/
void syscall_breakpoint_handler(void) {
    // Retrieve the old area, where the previous state is saved and extrapolate the exception code
    old_area = (state_t*) OLD_AREA_SYSCALL;
    unsigned int exCode = getExCode(old_area);

    // Bring back the PC to the previous instruction
    PC_REG(old_area) -= WORDSIZE;

    // Checsks if the code is for a syscall and not a breakpoint
    if (exCode == SYSCALL_CODE) {  
        unsigned int numberOfSyscall = GET_A0_REG(old_area);
        syscallDispatcher(numberOfSyscall);
    }

    // Else is a breakpoint 
    else if(exCode == BREAKPOINT_CODE)
        PANIC();

    // Unrecognized code for this handler
    else         
        PANIC();
}