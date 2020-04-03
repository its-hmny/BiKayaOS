#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "scheduler.h"
#include "pcb.h"


// Ready queue of the scheduler
struct list_head ready_queue;
// Current process selected to be executed
pcb_t *currentProcess = NULL;


/*
    This function is called by the scheduler after a process is chosen
    for the execution and simply increment by one the priority of all the excluded

    return: void
*/
HIDDEN void aging(void) {
    struct list_head *tmp = NULL;

    list_for_each(tmp, &ready_queue) {
        pcb_t *currentPCB = container_of(tmp, pcb_t, p_next);
        (currentPCB->priority)++;
    }
}


/*
    Prepares the ready queue and sets the scheduer to be exeuted

    return: void
*/
void scheduler_init(void) {
    initPcbs();
    mkEmptyProcQ(&ready_queue);
}


/*
    Adds a new process to the scheduler, checks the arguments first

    p: the PCB pointer to be added to the scheduler
    return: void
*/
void scheduler_add(pcb_t *p) {
    if (p != NULL) {
        p->original_priority = p->priority;
        insertProcQ(&ready_queue, p);
    }
}


/*
    The scheduler main function, each time that is called put back the currentProc in
    the ready queue and the chose a new process to be executed. If the ready queue is 
    empty then HALT the system else load the chosen process but before ages the priority
    of the excluded and set the currentProc timeslice
*/
void scheduler(void) {
    
    // If no process is ready, shut off the system (will be idle in future)
    if (emptyProcQ(&ready_queue))
       HALT();
    
    else {
        // If a process executed before puts it back in the queue
        if (currentProcess != NULL)
            scheduler_add(currentProcess);
        
        
        // Extracts a new process, restores its priority and ages all the excluded
        currentProcess = removeProcQ(&ready_queue);
        currentProcess->priority = currentProcess->original_priority;
        aging();

        // Loads the state and executes the chosen process but before sets the time slice
        setIntervalTimer();
        LDST(&currentProcess->p_s);
    }
}


// Returns a pointer to the ready queue
struct list_head* getReadyQ(void) {
    return(&ready_queue);
}

// Returns the current executing process
pcb_t* getCurrentProc(void) {
    return(currentProcess);
}


// Sets the current process (usually used to set it to NULL)
void setCurrentProc(pcb_t *proc) {
    currentProcess = proc;
}


/*
    Sets the Interval Timer in both architechture to the defined TIMESLICE

    return: void
*/
void setIntervalTimer(void) {
    #ifdef TARGET_UMPS
    memaddr *intervalTimer = (memaddr*) INTERVAL_TIMER;
    *intervalTimer = TIME_SLICE;
    #endif
    
    #ifdef TARGET_UARM
    setTIMER(TIME_SLICE);
    #endif
}


/*
    Set the Interval Timer in both architechture to a choosen timeslice
    that could be different from the default timeslice (3 milliseconds)

    time: the new interval that is desired to be set
    return: void
*/
void setTimerTo(unsigned int time) {
    // Timer setter on uMPS
    #ifdef TARGET_UMPS
    memaddr *intervalTimer = (memaddr*) INTERVAL_TIMER;
    *intervalTimer = time;
    #endif
    
    // Timer setter on uARM
    #ifdef TARGET_UARM
    setTIMER(time);
    #endif
}


/*
    Returns the current value in the Interval Timer as an unsigned int

    return: the current timer value
*/
unsigned int getIntervalTimer(void) {
    // Get the current elapsed time since the last timer ssetting uMPS
    #ifdef TARGET_UMPS
    memaddr *intervalTimer = (memaddr*) INTERVAL_TIMER;
    return (*intervalTimer);
    #endif
    
    // Get the current elapsed time since the last timer ssetting uARM
    #ifdef TARGET_UARM
    return (getTIMER());
    #endif
}