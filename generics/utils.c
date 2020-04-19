#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "../process/scheduler.h"
#include "./utils.h"



/*
    Wipe all the memory location from the starting addres until a specified
    wiping size is reached (moreless like _memset)

    memaddr: the starting memory address
    size: the size in bytes of the area that has to be wiped
*/
void wipe_Memory(void *memaddr, unsigned int size) {
    unsigned char* tmp_p = memaddr;
    
    while(size--)
        *tmp_p++ = (unsigned char) 0;
}


/*
    Initialize a new area for exception handling pourpose, sets all the option of the state registers,
    initialize stack pointer and program counter and so on

    handler: the function that has to be executed when a excption is reached
    RRF_addr: the starting memory location of the new area that has to be initialized
*/
void initNewArea(memaddr handler, memaddr RRF_addr) {
    state_t *newArea = (state_t*) RRF_addr;
    wipe_Memory(newArea, sizeof(state_t));

    // Set the state of the handler with disabled interrupt, kernel mode and so on (status register)
    process_option execpt_handler_option = EXC_HANDLER_PROC_OPT;
    setStatusReg(newArea, &execpt_handler_option);
    
    setPC(newArea, handler);
    
    setStackP(newArea, (memaddr)_RAMTOP);
}


#ifdef TARGET_UMPS
void setStatusReg(state_t *proc_state, process_option *option) {
    STATUS_REG(proc_state) |= option->interruptEnabled;
    // In uMPS LDST loses the first interrupt bit so the 2nd as always to be setted as backup
    STATUS_REG(proc_state) |= (option->interruptEnabled << IEP_SHIFT);
    STATUS_REG(proc_state) |= (option->kernelMode << KM_SHIFT);
    STATUS_REG(proc_state) |= (option->interrupt_mask << INTERRUPT_MASK_SHIFT);
    STATUS_REG(proc_state) |= (option->virtualMemory << VIRT_MEM_SHIFT);
    STATUS_REG(proc_state) |= (option->timerEnabled << PLT_SHIFT);
}
#endif

#ifdef TARGET_UARM
void setStatusReg(state_t *proc_state, process_option *option) {
    STATUS_REG(proc_state) = (option->kernelMode) ? (STATUS_SYS_MODE) : (STATUS_USER_MODE); 
    STATUS_REG(proc_state) = (option->interruptEnabled) ? (STATUS_ENABLE_INT(STATUS_REG(proc_state))) : (STATUS_DISABLE_INT(STATUS_REG(proc_state)));
    proc_state->CP15_Control = (option->virtualMemory) ? (CP15_ENABLE_VM(proc_state->CP15_Control)) : (CP15_DISABLE_VM(proc_state->CP15_Control));
    STATUS_REG(proc_state) = (option->timerEnabled) ? (STATUS_ENABLE_TIMER(STATUS_REG(proc_state))) : (STATUS_DISABLE_TIMER(STATUS_REG(proc_state)));
}
#endif


/*
    Sets the Program Counter register to the given entry point (should be a function)

    process: the state_t that has to be setted
    function: the entry point of the function (e.g. (memaddr)term_print)
    retunr: void
*/
void setPC(state_t *process, memaddr function) {
    PC_REG(process) = function;
}


/*
    Set the stack pointer of a given processor state to the given memory location

    process: the state that has to be set
    memLocation: the memory location wich the SP has to point
*/
void setStackP(state_t *process, memaddr memLocation) {
    SP_REG(process) = memLocation;
}


// Returns the exception code from the cause registrer in the old area
unsigned int getExCode(state_t *oldArea) {
    unsigned int causeReg = CAUSE_REG(oldArea);
    return(CAUSE_GET_EXCCODE(causeReg));
}


/*
    Function that clones a processor state into another processor state,
    in theory it could be used for other pourpose but is strongly advised not to

    process_state: the state that has to be overridden
    old_area: the process state that has to be cloned
    size: the size of a process state in the current compiling architechture 
          (also used to prevent random memory writing)
    return: void
*/
void cloneState(state_t *process_state, state_t *old_area, unsigned int size) {
    char *copy = (char *) process_state, *to_be_copied = (char *) old_area;
    while(size--) {
        *copy = *to_be_copied;
        copy++, to_be_copied++;
    }
}


/*
    This function initialize the time_t struct of a PCB, if the struct has been
    already initialized then the function stops & returns

    process_time: the time_t structure to be initialized
    return: void
*/
void init_time(time_t *process_time) {
    // The time struct is already set
    if (process_time->activation_time) 
        return;

    // Set it up for the first time
    process_time->kernelmode_time = 0;
    process_time->usermode_time = 0;
    process_time->activation_time = TOD_LO / TIME_SCALE;
    process_time->last_update_time  = process_time->activation_time;
}


/*
    This function takes two integer as "instant" and then calculate the time difference
    between them, divides it to the scale (converting from clock cycle to second) and then
    add the elapsed time to the given time_counter. Also it sets the last_update field to current time

    time_counter: the counter that has to be updated
    last_update: the instant in which the last update occurred
    current_time: the instant of time at the moment of the call
*/
void update_time(unsigned int option, unsigned int current_time) {
    // Retrieve the current process, and check his validity 
    pcb_t *tmp = getCurrentProc();
    if (tmp == NULL)
        PANIC();
    
    // Retrieve the needed fields in the time_t structure
    unsigned int *time_counter = option ? &tmp->p_time.kernelmode_time : &tmp->p_time.usermode_time;
    unsigned int *last_update = &tmp->p_time.last_update_time;
    
    // Get the elapsed time also in case of TOD_LO overflow for kernel/user time
    unsigned int elapsed_clocks = (current_time > *last_update) ? current_time - *last_update : 0;
    *time_counter += (elapsed_clocks / TIME_SCALE);
    *last_update = current_time;

    // Update then the time since activation
    activation_time_updt(tmp, current_time);
}

void activation_time_updt(pcb_t *proc, unsigned int current) {
    // Retrieve the current process, and check his validity
    if (proc == NULL)
        PANIC();
    
    // Get the time_t struct and a copy of the last_update_time data
    time_t *proc_time = &proc->p_time;
    unsigned int last_update = proc_time->last_update_time;
    // Calculate the interval between the last update and this one
    proc_time->activation_time = (current > last_update) ? current - last_update : 0;
    // Sets the new instant of the update
    proc_time->last_update_time = current;
}