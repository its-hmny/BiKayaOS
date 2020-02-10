#ifndef ASL_H
#define ASL_H

#include "asl.h"
#include "../include/types_bikaya.h"
#include "../include/const.h"
#include "../include/listx.h"

#define HIDDEN static;
semd_t ASL_arr[MAXPROC];

HIDDEN LIST_HEAD(fsl);
HIDDEN LIST_HEAD(asl);

/* ASL handling functions */
semd_t* getSemd(int *key);
void initASL();

int insertBlocked(int *key,pcb_t* p);
pcb_t* removeBlocked(int *key);
pcb_t* outBlocked(pcb_t *p);
pcb_t* headBlocked(int *key);
void outChildBlocked(pcb_t *p);

#endif
