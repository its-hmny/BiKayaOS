#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "scheduler.h"
#include "pcb.h"
#define HIDDEN static

#define TIME 3000
#define TIME_SLICE (TIME * TIME_SCALE)


// Ready queue of the scheduler
struct list_head ready_queue;
// Current process selected to be executed
pcb_t *currentProcess = NULL;


/*
    Thia function is called by the scheduler after a process is chosen
    for the execution and simply increment by one the priority of all the excluded
*/
HIDDEN void aging(void) {
    struct list_head *tmp = NULL;

    list_for_each(tmp, &ready_queue) {
        pcb_t *currentPCB = container_of(tmp, pcb_t, p_next);
        (currentPCB->priority)++;
    }
}


// Prepares the ready queue and sets the scheduer to be exeuted
void scheduler_init(void) {
    mkEmptyProcQ(&ready_queue);
}


// Adds a new process to the scheduler, checks the arguments first
void scheduler_add(pcb_t *p) {
    if (p != NULL) {
        p->original_priority = p->priority;
        insertProcQ(&ready_queue, p);
    }
}


// Choose the next process to be executed
void scheduler() {

    // If no process is ready, wait till one is
    if (emptyProcQ(&ready_queue)) 
        while(1);
    
    else {
        // If a process executed before puts it back in the queue
        if (currentProcess != NULL)
            scheduler_add(currentProcess);
        
        
        // Extracts a new process, restores its priority and ages all the excluded
        currentProcess = removeProcQ(&ready_queue);
        currentProcess->priority = currentProcess->original_priority;
        aging();

        // Loads the state and executes the chosen process but before sets the time slice
        setTIMER(TIME_SLICE);
        LDST(&currentProcess->p_s);
    }
}


// Returns the current executing process
pcb_t* getCurrentProc(void) {
    return(currentProcess);
}


// Return a ready queue pointer
struct list_head* getReadyQ(void) {
    return(&ready_queue);
}