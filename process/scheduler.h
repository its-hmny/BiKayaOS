#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/types_bikaya.h"

extern struct list_head ready_queue;
extern struct pcb_t* currentP;

void scheduler();
void insertProcReady(struct pcb_t* newProc);
void removeProcReady(struct pcb_t* rmProc);
void aging();
void scheduler_add(pcb_t *process);

#endif