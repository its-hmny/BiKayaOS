#include "../include/system_const.h"
#include "../include/types_bikaya.h"
#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "../process/asl.h"
#include "../process/pcb.h"
#include "syscall_breakpoint.h"


// A pointer to the old area, used to retrieve info about the exception
HIDDEN state_t *old_area = NULL;

/* ================ SYSCALL DEFINITION ================ */

HIDDEN void syscall1(unsigned int *user, unsigned int *kernel, unsigned int *wallclock) {
    // Update all the data before returning
    update_time(KER_MD_TIME, TOD_LO);
    // Get the time_t struct
    time_t *data = &getCurrentProc()->p_time;

    *user = data->usermode_time;
    *kernel = data->kernelmode_time;
    *wallclock = data->activation_time;
}
/*
    This syscall create a new process, allocating the PCB, saving the given
    state inside it and setting up the new process in the parent's child list.
    The function takes care also of adding the process to the scheduler, 
    and then calling it (round-robin scheduler).

    statep: the state of the newborn process
    priority: the newborn priority
    cpid: the newborn pid that is as well a pointer to the PCB itself
    return: 0 on success, -1 on failure
*/
HIDDEN void syscall2(state_t* statep, int priority, void** cpid) {
    pcb_t *new_proc = allocPcb();
    pcb_t *parent = getCurrentProc();

    // Error during allocation, error code returned
    if (new_proc == NULL || statep == NULL || cpid != NULL || parent == NULL)
        SYS_RETURN_VAL(old_area) = FAILURE;

    // Set the given state to the new process
    cloneState(&new_proc->p_s, statep, sizeof(state_t));

    // Set priority and insert the PCB in the father's child node
    new_proc->priority = priority;
    insertChild(parent, new_proc);

    // Insert the new process in the ready queue and sets the pid
    scheduler_add(new_proc);
    *cpid = new_proc;
    
    SYS_RETURN_VAL(old_area) = SUCCESS;
}


/*
    This syscall terminates the process given as input (pid), removing recursively
    from the ASL, the ready queue and the father's child list.
    If pid is NULL then the current process/caller is killed.
    This is done for all the descendants of the given PCB (Sons, Grandsons, etc).

    pid: a pointer to the process to terminate
    return: 0 on success, -1 on failure
*/
HIDDEN void syscall3(void* pid) {
    pcb_t* proc = pid ? pid : getCurrentProc(); 
    unsigned int sys_status = 0;
    
    // The function has no process to kill
    if (proc == NULL)
        SYS_RETURN_VAL(old_area) = FAILURE;

    // Removes the root from father's child list
    outChild(proc);
    // Removes it from the semd or ready_queue one of them must be true, else error
    sys_status |= (proc == outBlocked(proc));
    sys_status |= (proc == outProcQ(getReadyQ(), proc));

    struct list_head *tmp = NULL;

    list_for_each(tmp, &proc->p_child) {
        // Recursive call on the childs
        syscall3(container_of(tmp, pcb_t, p_sib));
        // Check the return code of the recursive call
        if (SYS_RETURN_VAL(old_area) == FAILURE)
            return;
    }

    freePcb(proc);

    SYS_RETURN_VAL(old_area) = sys_status ? SUCCESS : FAILURE;
}


/*
    This syscall assign the current process pid and his parent pid
    to the given parameter (after checking that both are not NULL)

    pid: the mem location where to save the pid
    ppid: the mem location where to save the curr. proc. parent pid
    retunr: void;
*/
HIDDEN void syscall8(void** pid, void** ppid){
    pcb_t *current = getCurrentProc();
    *pid = pid ? current : NULL;
    *ppid = ppid ? current->p_parent : NULL;
}



/* ========== SYSCALL & BREAKPOINT HANDLER ========== */

/* 
    This function takes the syscall number and call the appropriate system call,
    eventually initializing the args of each syscall. If a syscall is not recognized
    or implemented then issue a Kernel Panic

    sysNumber: the syscall number retrieved from the Old Area
    return: void
*/
void syscallDispatcher(unsigned int sysNumber) {
    switch (sysNumber) {
        case 1:
            syscall1((unsigned int*)SYS_ARG_1(old_area), (unsigned int*)SYS_ARG_2(old_area), (unsigned int*)SYS_ARG_3(old_area));
            break;

        case 2:
            syscall2((state_t*)SYS_ARG_1(old_area), (int)SYS_ARG_2(old_area), (void**)SYS_ARG_3(old_area));
            break;

        case 3:
            // Kill the current process wich has called the syscall
            syscall3((void*)SYS_ARG_1(old_area));
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
            syscall8((void**)SYS_ARG_1(old_area), (void**)SYS_ARG_2(old_area));
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
    // At first update the user_time of execution
    update_time(USR_MD_TIME, TOD_LO);
    // Retrieve the old area, where the previous state is saved and extrapolate the exception code
    old_area = (state_t*) OLD_AREA_SYSCALL;
    unsigned int exCode = getExCode(old_area);

    // Sets the PC to the next instruction in uMPS
    #ifdef TARGET_UMPS
    PC_REG(old_area) += WORDSIZE;
    #endif

    // Checsks if the code is for a syscall and not a breakpoint
    if (exCode == SYSCALL_CODE) {  
        unsigned int numberOfSyscall = SYSCALL_NO(old_area);
        syscallDispatcher(numberOfSyscall);
    }

    // Else is a breakpoint 
    else if(exCode == BREAKPOINT_CODE)
        PANIC();

    // Unrecognized code for this handler
    else         
        PANIC();
    
    // At last update the kernel mode execution time
    update_time(KER_MD_TIME, TOD_LO);
}