#include "../include/const.h"
#include "../include/listx.h"
#include "../include/types_bikaya.h"
#include "pcb.h"

#define HIDDEN static

HIDDEN LIST_HEAD(pcbFree_queue);

/*This method cleans the pcb from the previous data that was stored in it*/
HIDDEN void wipe_Pcb(void *block, unsigned int val, unsigned int size) {
    if (size) {
        char *toWipe = block;
        
        while(size--)
            *toWipe++ = val;
    }
}

void initPcbs(void) {
    pcb_t pcbTmp_arr[MAXPROC];
    unsigned int i;
    
    for (i = 0; i < MAXPROC; i++) {
        list_add_tail(&pcbTmp_arr[i].p_next, &pcbFree_queue);
    }
}

void freePcb(struct pcb_t *p) {
    /*Returns the given pcb to a the pcbFree_queue*/
    list_add_tail(&p->p_next, &pcbFree_queue);
}

pcb_t *allocPcb(void) {
    /*Returns NULL if the pcbFree_List is empty (no free pcbs avaiable)*/
    if (list_empty(&pcbFree_queue))
        return (NULL);

    /*Delete the pcb from the pcbFree_queue, obtain the pcb_t struct with "container_of" and return it*/
    else {
        pcb_t *freePcb = container_of(pcbFree_queue.next, struct pcb_t, p_next);
        list_del(pcbFree_queue.next);
        /*Before returning the Pcb it sets all the values to zeros*/
        wipe_Pcb(freePcb, 0, sizeof(freePcb));
        return(wipe_Pcb);
    }
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