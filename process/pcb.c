#include "../include/const.h"
#include "../include/listx.h"
#include "../include/types_bikaya.h"
#include "pcb.h"

#define HIDDEN static

pcb_t pcbTmp_arr[MAXPROC];
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
        pcb_t *freePcb = container_of(pcbFree_queue.next, pcb_t, p_next);
        list_del(pcbFree_queue.next);
        /*Before returning the Pcb it sets all the values to zeros*/
        wipe_Pcb(freePcb, 0, sizeof(pcb_t));
        return(freePcb);
    }
}

void mkEmptyProcQ(struct list_head *head) {
    HIDDEN LIST_HEAD(pcbActive_queue);
    head = &pcbActive_queue;
}

int emptyProcQ(struct list_head *head) {
    return(list_empty(head));
}

void insertProcQ(struct list_head *head, pcb_t *p) {
    struct list_head *tmp;
    pcb_t *last_examined_pcb;
    /*Initial check that the parameter are correct*/
    if (head == NULL || p == NULL)
        return;

    /*A list with only one element is always sorted (semiquote)*/
    else if (list_empty(head))
        list_add(&p->p_next, head);

    /*Insert the element maintaining the sorting property of the queue*/
    else {
        list_for_each(tmp, head) {
            last_examined_pcb = container_of(tmp, pcb_t, p_next);

            /*If the PCB has to stay in the middle of the queue, adds it in between and returns*/
            if (p->priority > last_examined_pcb->priority) {
                list_add(&(p->p_next), tmp->prev);
                return;
            }
        }

        /*If the cicle loops til the end then the pcb has to be put in the queue tail*/
        list_add_tail(&p->p_next, head);
    }
}

pcb_t *headProcQ(struct list_head *head) {
    if ((head == NULL) || list_empty(head))
        return (NULL);

    return(container_of(head->next, pcb_t, p_next));
}

pcb_t *removeProcQ(struct list_head *head) {
    pcb_t *rmdPCB = headProcQ(head);
    
    if (rmdPCB != NULL)    
        list_del(&(rmdPCB->p_next));
    
    return (rmdPCB);
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p) {
    struct list_head *tmp;
    pcb_t *block;

    if (head == NULL || list_empty(head) || p == NULL)
        return (NULL);

    list_for_each(tmp, head) {
        block = container_of(tmp, pcb_t, p_next);
        
        if (p == block) {
            list_del(tmp);
            return (block);
        }
    }

    return (NULL);
}

int emptyChild(pcb_t *this) {
 //return 1 (TRUE) if pcb *this haven't got a child,otherwise return 0(FALSE)
    if (this->p_child == NULL)
        return 1;
    else 
        return 0;
}

void insertChild(pcb_t *prnt, pcb_t *p) {
    p->p_parent = prnt;
    p->p_sib = prnt->p_child;
    prnt->p_child = p;

    }

pcb_t *removeChild(pcb_t *p) {
    if(p->p_child == NULL)
        return(NULL);
    else{
        pcb_t *tmp = p;
        pcb_t *pparent = p->p_parent;
        pparent->p_child = tmp->p_sib;
        tmp->p_sib = NULL;
        tmp->p_parent = NULL;
        return tmp; 

    }
}

pcb_t *outChild(pcb_t *p) {
    if(p->p_parent == NULL)
        return(NULL);
    else{
            pcb_t* pparent = p->p_parent;
            pcb_t* pp = pparent->p_child;
            pcb_t* pp1 = pp->p_sib;
            if(pp == p){
                return removeChild(p);
            }else if(pp ->p_sib == p){
                
                pp1 = pp1->p_sib;
                return p;
            }else {
                while(pp1 ->p_sib != NULL){
                    if(pp1 == p)
                        pp->p_sib = pp1->p_sib;
                    pp = pp1;
                    pp1= pp1->p_sib;
                    
                }
                return p;
            }
        }
    }
}