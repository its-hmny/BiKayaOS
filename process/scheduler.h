#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/types_bikaya.h"

void scheduler_init(void);
void scheduler_add(pcb_t *p);
void scheduler();
pcb_t* getCurrentProc(void);
struct list_head* getReadyQ(void);

#endif