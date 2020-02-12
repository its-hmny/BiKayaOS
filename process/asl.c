#include "../include/types_bikaya.h"
#include "../include/listx.h"
#include "../include/const.h"
#include "pcb.h"
#include "asl.h"

#define HIDDEN static;

semd_t semdTmp_arr[MAXPROC];
HIDDEN LIST_HEAD(semdFree_list); //Free semaphores list
HIDDEN LIST_HEAD(semdAct_list); // Active semaphore list

/*
    Auxiliary function to remove a semaphore with an empty process queue from 
    the active semaphore list, it removes it only if empty
    
    semd: the semaphore that as to be removed if empty
    return: void
*/
HIDDEN void rmvEmptySemd(struct semd_t* semd){
    if (list_empty(&(semd->s_procQ))) { 
        list_del(&(semd->s_next)); 
        list_add_tail(&(semd->s_next), &semdFree_list);
    }
}

/*
    This function returns the semaphore in the active semd list that corresponds to the
    key given as parameter.

    key: the key associated to the semd we want
    return: the semaphor associated to the key if found, else NULL
*/
semd_t* getSemd(int *key) {
    struct list_head *pos;
    semd_t *tmp;
    
    list_for_each(pos, &semdAct_list){
        tmp = container_of(tmp , semd_t , s_next);
        if (tmp->s_key == key)
            return tmp;
    }
    
    return(NULL);
}

/*
    Initializes the semaphore list by inserting all the semd in the 
    semd free list. 
*/
void initASL(void) {
    unsigned int i;
    
    for(i = 0; i < MAXPROC ; i++) {
        INIT_LIST_HEAD(&(semdTmp_arr[i].s_procQ)); //Initialize s_procQ to empty list
        list_add_tail(&(semdTmp_arr[i].s_next), &semdFree_list);
    } 
}

/*
    Inserts the given PCB p in the semd with the matching key, if a semd with the same key
    doesn't exist, the function tries to allocate a new one if avaiable else returns NULL

    key: the key corresponding to the semaphor in wich we have to add the PCB
    p: the PCB desired to be added
    return: 1 on success, on fail
*/
int insertBlocked(int *key, pcb_t* p) {
     semd_t *tmp = getSemd(key); //Find the semd through his own key
     pcb_t *proc;
    
    if (tmp == NULL) {
        if (! list_empty(&semdFree_list)) {
            tmp = container_of(list_next(&semdFree_list), semd_t, s_next); //Obtain the first semd in the free queue
            list_del(&(tmp->s_next));
            list_add_tail(&(tmp->s_next), &semdAct_list);
            tmp->s_key = key;
            return (TRUE);
        } 
        else
            return (FALSE);
    }

    list_add_tail(&(p->p_next), &(tmp->s_procQ));
    return (TRUE);  
}

/*
    Remove the first PCB blocked on the semaphor with the corresponding key
    after the removal of the process from the queue checks that the queue hasn't become empty
    and if happened return the semd to the free list (rmvEmptySemd)

    key: the key associated to the semaphor
    return: the first PCB of that semaphore, NULL if error happened
*/
pcb_t* removeBlocked(int *key) {
    semd_t *semd = getSemd(key);
    struct list_head *pos;
    pcb_t *proc;
    
    if (semd == NULL || list_empty(&semd->s_procQ))
        return (NULL);
        
    pos = list_next(&semd->s_procQ);
    proc = container_of(pos, pcb_t , p_next);
    list_del(pos);

    rmvEmptySemd(semd);

    return(proc);   
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
    list_for_each(pos, &(semd->s_procQ)) {  
        tmp = container_of(pos, pcb_t, p_next);
        
        if (p == tmp) { 
            list_del(&(tmp->p_next));
            rmvEmptySemd(semd); //If the semd->s_procQ became an empty list, removes semd from the semdActive_list
            return (tmp);
        }
    }
}

/*
    This function gets the semaphor through the semkey, checks for args and the semd
    to be not NULL (error checking) and then returns the first PCB in the blocked sem queue
    but WITHOUT removing it from the queue.

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
    if (root != NULL && ! list_empty(&(root->p_child))) {
        struct list_head *tmp;

    //Obtains the child PCB with container_of and recursively removes the grandchild of the actual root
        list_for_each(tmp, &(root->p_child)) {
            pcb_t *child = container_of(tmp, pcb_t, p_sib);
            outChildBlocked(child);
            freePcb(child);
        }
    }

    //Then deallocates the root pcb
    freePcb(root);
}
