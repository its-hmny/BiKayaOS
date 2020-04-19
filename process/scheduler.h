#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "../include/types_bikaya.h"

void scheduler_init(void);
void scheduler_add(pcb_t *p);
void scheduler(void);
pcb_t* getCurrentProc(void);
void setCurrentProc(pcb_t *proc);
void setParentProc(pcb_t *proc);
struct list_head* getReadyQ(void);
void setIntervalTimer(void);
void setTimerTo(unsigned int time);
unsigned int getIntervalTimer(void);

#endif