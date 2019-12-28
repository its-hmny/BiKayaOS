#include "../include/types_bikaya.h"
#include "../include/const.h"
#include "../include/listx.h"


void initPcbs(void) {}

void freePcb(pcb_t *p) {}

pcb_t *allocPcb(void) {
    return(NULL);
}

void mkEmptyProcQ(struct list_head *head) {}

int emptyProcQ(struct list_head *head) {
    return(NULL);
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