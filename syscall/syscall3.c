#include "syscall3.h"

//this function terminate current process and all its offspring (progenie), remove them from the ready queue
void syscall3(pcb_t* pid){
    pcb_t *tmp = pid;

    outChildBlocked(pid);

    semd_t *semd = getSemd(pid->p_semkey);
    rmvEmptySemd(semd); //if the queue after the removal becomes empty the function deletes also the semaphore descriptor from the list and insert it back in the free list
    
    while(!emptyChild(tmp)){
        removeProcQ(&tmp->p_child);
    }

    removeProcQ(&tmp);

    kill(pid);
}

//this function killa tutti i testimoni di geova, anche i figli dei figli.  DIO PORCO
void kill(pcb_t* root){
    pcb_t *tmp = root;

    tmp = removeChild(tmp);

    while(tmp!=NULL){
        if(emptyChild(tmp))
            freePcb(tmp);
        else
            kill(tmp);
    }       
}