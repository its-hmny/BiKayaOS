#include "../include/types_bikaya.h"
#include "../include/listx.h"
#include "../include/const.h"
#include "asl.h"

#define HIDDEN static;

struct list_head *semd_h; //sentinella asl
struct list_head *semdFree_h; //sentinella lista libera


semd_t* getSemd(int *key) {
    return(NULL);
}

void initASL() {}

int insertBlocked(int *key,pcb_t* p) {
    return(0);
}

pcb_t* removeBlocked(int *key) {
    return(NULL);
}

/*
    This function removes the PCB pointed by p from the semaphor's queue (found with the semkey)
    where it's blocked, then if the queue after the removal becomes empty the function deletes
    also the semaphore descriptor from the list and insert it back in the free list.

    p: the PCB wich has to be removed from the queue
    return: the PCB removed if found, NULL if not found 
*/
pcb_t* outBlocked(pcb_t *p) {
    semd_t *semd = getSemd(p->p_semkey);
    struct list_head *pos;

    if (p == NULL && semd == NULL)
        return NULL;

    // Cicles till the PCB is found
    list_for_each(pos,&(semd->s_procQ)) {  
        //USA CONTAINER OF PER RICAVARE IL PCB
        //Luca questo ramo if è sbagliato, anche la condizione
        if (pos->next == p) { }
        
    }

    // Checks if the list has been emptied, fare a parte come HIDDEN
    if (list_empty(&(semd->s_procQ))) { 
        list_del(semd); 
        list_add_tail(semd, semdFree_h);
    }

    return(pos); //Ritorni un list_head invece che un pcb_t
}

/*
    This function gets the semaphor through the semkey, checks for args and the sem
    to be not NULL (error checking) and then returns the first PCB in the blocked sem queue

    key: the semaphore's key of wich we want to obtain the blocked PCB
    return: the first blocked PCB on the semaphore queue if found, else NULL 
*/
pcb_t* headBlocked(int *key) {
    semd_t* semd = getSemd(key);
    struct list_head* pos;
    
    //Arguments check 
    if (semd == NULL || list_empty(&semd->s_procQ)) 
        return NULL;

    //Takes the first element in the queue and returns it
    pos = list_next(&(semd->s_procQ));
    return (container_of(pos, pcb_t, p_next));
}

/*
    This function removes the PCB p from his semaphor queue, then iterates through
    all of his own tree (wich root is p himself) removing recursively from their own queue
    his sons, grandsons and so on

    p: is the PCB root from wich we want to start removing (the root)
    return: void
*/
void outChildBlocked(pcb_t *p) {
    struct pcb_t *root = outBlocked(p);
  
    //Check if the root has childs
    if (! list_empty(&(root->p_child))) {
        struct list_head *tmp;

    //Obtains the child PCB with container_of and recursively removes the grandchild of the actual root
        list_for_each(tmp, &(root->p_child)) {
            pcb_t *child = container_of(tmp, pcb_t, p_sib);
            outChildBlocked(child);
            //Dove li salviamo tutti sti PCB deallocati??
        }
    }
}
