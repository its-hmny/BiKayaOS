#include "../include/const.h"
#include "../include/listx.h"
#include "../include/types_bikaya.h"
#include "asl.h"

#define HIDDEN static;
semd_t ASL_arr[MAXPROC];

HIDDEN LIST_HEAD(semdFree); //free semaphore list
HIDDEN LIST_HEAD(asl); // Active semaphore list

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
        if(list_empty(&semd->s_procQ)) //delete list if is empy
            list_del(&semd->s_next);
        return(proc);   
    return(NULL);
}

pcb_t* outBlocked(pcb_t *p) {
    return(NULL);
}

pcb_t* headBlocked(int *key) {
    return(NULL);
}

void outChildBlocked(pcb_t *p) {}