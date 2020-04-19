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

HIDDEN int syscall2(state_t *statep, int priority, void ** cpid){
    pcb_t *new_proc = allocPcb();

    if(new_proc != NULL){   // new process allocated successfully 
        // Set new_proc state 
        cloneState(&new_proc->p_s,statep,sizeof(statep));

        // Set priority and original priority 
        new_proc->priority=priority;

        // Set new_proc as a child of the current process 
        insertChild(getCurrentProc(), new_proc);

        // Insert the new process into the scheduler
        scheduler_add(new_proc);

        if (cpid)   //cpid contain the id of new the process
            *((pcb_t **)cpid) = new_proc;

        return 0;
    }
    else
        return -1; 
}

HIDDEN int syscall3(void * pid) {
    pcb_t* proc = NULL; 
    
    if (pid == NULL)// We are in the current process
        proc = getCurrentProc();
    else
        proc = (pcb_t *)pid; 

        // Removes the root from father's child list
        outChild(proc);
        // Removes it from the semaphor's queue, if present
        outBlocked(proc);
        // Removes it from the ready queue, if present
        outProcQ(getReadyQ(), proc);

        struct list_head *tmp = NULL;

        list_for_each(tmp, &proc->p_child)
            syscall3(container_of(tmp, pcb_t, p_sib));

        freePcb(proc);


}


HIDDEN void syscall8(void ** pid, void ** ppid){
    if(pid != 0)
      setCurrentProc(*pid);
    if(ppid != 0)
      setParentProc(*ppid);
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
            syscall2(GET_A1_REG(old_area),GET_A2_REG(old_area),GET_A3(old_area)); //non ne sono sicuro (cast)
            break;

        case 3:
            // Kill the current process wich has called the syscall
            syscall3((void *)GET_A1_REG(old_area));
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
            //assigns the ID of the current process to *pid  and the identifier of the parent process to *ppid
            syscall8((void **)GET_A1_REG(old_area), (void **)GET_A2_REG(old_area));
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

    // Sets the PC to the next instruction in uMPS
    #ifdef TARGET_UMPS
    PC_REG(old_area) += WORDSIZE;
    #endif

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