#include "../include/const.h"
#include "../include/listx.h"
#include "pcb.h"

#define HIDDEN static

HIDDEN LIST_HEAD(pcbFree_queue);

/*This method cleans the pcb from the previous data that was stored in it*/
HIDDEN void wipe_Pcb(pcb_t *block) {
    WIPE_LIST_HEAD(block->p_child);
    WIPE_LIST_HEAD(block->p_next);
    WIPE_LIST_HEAD(block->p_sib);
    block->p_parent = NULL;
    block->p_semkey = NULL;
    block->priority = 0; /*Check if 0 is a correct value for this field*/
}

void initPcbs(void) {
    pcb_t pcbTmp_arr[MAXPROC];
    unsigned int i;
    
    for (i = 0; i < MAXPROC; i++) {
        list_add_tail(&pcbTmp_arr[i].p_next, &pcbFree_queue);
    }
}

void freePcb(pcb_t *p) {
    /*list_del automatically removes it from the list it belongs (in this case the active pcb list)*/
    list_del(&(p->p_next));
    /*Then return the same pcb to a the pcbFree_queue*/
    list_add_tail(&(p->p_next), &(pcbFree_queue));
}

pcb_t *allocPcb(void) {
    /*Returns NULL if the pcbFree_List is empty (no free pcbs avaiable)*/
    struct list_head *freeElement = list_next(&pcbFree_queue);

    /*Delete the pcb from the pcbFree_queue, obtain the pcb_t struct with "container_of" and return it*/
    if (freeElement != NULL) {
        list_del(freeElement); 
        pcb_t *freePCb = container_of(freeElement, struct pcb_t, p_next);
        
        /*Before returning the Pcb it sets all the values to zeros*/
        wipe_Pcb(freePCb);
        return (freePcb);
    }
    
    else return (NULL);
}

void mkEmptyProcQ(struct list_head *head) {
    HIDDEN LIST_HEAD(pcbActive_queue);
    head = &pcbActive_queue;
}

int emptyProcQ(struct list_head *head) {
    return(list_empty(head));
}

void insertProcQ(struct list_head *head, pcb_t *p) {}

pcb_t *headProcQ(struct list_head *head) {
    return(NULL);
}

pcb_t *removeProcQ(struct list_head *head) {
    return(NULL);
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p) {
    return(NULL);
}

int emptyChild(pcb_t *this) {
    return(NULL);
}

void insertChild(pcb_t *prnt, pcb_t *p) {}

pcb_t *removeChild(pcb_t *p) {
    return(NULL);
}

pcb_t *outChild(pcb_t *p) {
    return(NULL);
}