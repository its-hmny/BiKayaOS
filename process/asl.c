#include "../include/types_bikaya.h"
#include "../include/listx.h"
#include "../include/const.h"
#include "asl.h"

#define HIDDEN static;

semd_t ASL_arr[MAXPROC];

HIDDEN LIST_HEAD(semdFree); //free semaphore list
HIDDEN LIST_HEAD(asl); // Active semaphore list

struct list_head *semd_h; //sentinella asl
struct list_head *semdFree_h; //sentinella lista libera

void checkIsEmpty(struct semd_t* semd){
    if (list_empty(&(semd->s_procQ))) { 
        list_del(semd); 
        list_add_tail(semd, semdFree_h);
    }
}

semd_t* getSemd(int *key) {
    struct list_head *tmp;
    semd_t *skey;
    list_for_each(tmp, &asl){
        skey = container_of(tmp , semd_t , s_next);
        if( skey->s_key == key)
            return skey;
    }
    
    return(NULL);
}

void initASL() {
    unsigned int i;
    for(i=0; i< MAXPROC ; i++){
        INIT_LIST_HEAD(&ASL_arr[i].s_procQ); // both next & prev point to itself
        list_add( &ASL_arr[i].s_next,  &semdFree); } 
    }

int insertBlocked(int *key,pcb_t* p) {
     semd_t *semd = getSemd(key); // find the semd whit the same key
     pcb_t *proc;
    if(semd == NULL )  
        if (list_empty(list_next(&semdFree)))
            return TRUE;
        else
            { semd = container_of(list_next(&semdFree), struct semd_t, s_next);
              list_del(list_next(&semdFree)); // delete the head from the free list
              list_add_tail(&semd->s_next, &asl);
              semd->s_key = key; 
            }
        list_add_tail(&proc->p_next, &semd->s_procQ);
        
    
    return(0);
}

pcb_t* removeBlocked(int *key) {
    semd_t *semd = getSemd(key);
    pcb_t *proc;
    struct list_head *pos;

    if( semd != NULL)
        pos = list_next(&semd->s_procQ);
        proc = container_of(pos, pcb_t , p_next);
        list_del(pos);

       /* if(list_empty(&semd->s_procQ)) //delete list if is empy
            list_del(&semd->s_next); */

        checkIsEmpty(semd); //QUELLO APPENA COMMENTATO LO SOSTITUISCO CON QUESTO, ANCHE PERCHÈ NON LO METTEVA IN QUELLA LIBERA QUINDI MANCAVA QUELLA PARTE

        return(proc);   
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
     struct pcb_t *tmp;

    if (p == NULL && semd == NULL)
        return NULL;

    // Cicles till the PCB is found
    list_for_each(pos,&(semd->s_procQ)) {  
        struct pcb_t *pointer = container_of(pos,struct pcb_t,p_next);
        if (p == pointer) { 
            tmp=pointer;
        }
    }

    list_del(tmp);

    checkIsEmpty(semd);  // Checks if the list has been emptied, fare a parte come HIDDEN

    return(container_of(tmp,struct pcb_t,p_next)); 
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
