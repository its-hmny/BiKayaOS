#include "../include/types_bikaya.h"
#include "scheduler.h"

#define PTIME 3000          //time for process (milliseconds)

void scheduler_add(pcb_t* p){
//      Add pcb in the Ready_Queue
    
    insertProcQ(&ready_queue,p);    
}
void scheduler(){
//      Select the process to execute from the queue

    if(emptyProcQ(&ready_queue) && (currentProc == NULL)){
        while(1);
    }else{
        //context switch occurs
        if(currentProc != NULL){
            insertProcReady(currentProc);
        }

        //currentProc = removeProcReady(&ready_queue);
        aging();

    }
}

void insertProcReady(struct pcb_t* newProc){
    restorePriority(newProc);
    insertProcQ(&ready_queue, newProc);
}

void removeProcReady(struct pcb_t* p){
/*
    Remove p {type pcb_t*} from ready queue 
    if p is the current running process becomes NULL
*/

    outProcQ(&ready_queue,p);
    if(p == currentProc )
        currentProc = NULL;
}

void aging(){
//      Increase the priority of processes on ready queue

    struct list_head* list;
    struct pcb_t* p;

    if(emptyProcQ(&ready_queue) > 0 ){
        p = container_of(list, struct pcb_t,p_next);

        //not aging the idle process
        if(p->original_priority != 1)
            increasePriority(p);
        
        
    }
}

