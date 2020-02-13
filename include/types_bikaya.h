#ifndef _TYPES11_H
#define _TYPES11_H
#ifdef TARGET_UMPS
#include "uMPS/umps/types.h"
#endif
#ifdef TARGET_UARM
#define UARM_MACHINE_COMPILING
#include "uARM/uarm/uARMtypes.h"
#endif
#include "listx.h"

typedef unsigned int memaddr;

/* Process Control Block (PCB) data structure */
typedef struct pcb_t {
    /* Process queue fields */
    struct list_head p_next;

    /* Process tree fields */
    struct pcb_t *   p_parent;
    struct list_head p_child, p_sib;

    /* Processor state, etc */
    state_t p_s;

    /* Process priority */
    int priority;

    /* Key of the semaphore on which the process is eventually blocked */
    int *p_semkey;
} pcb_t;



/* Semaphore Descriptor (SEMD) data structure */
typedef struct semd_t {
    struct list_head s_next;

    /* Semaphore key */
    int *s_key;

    /* Queue of PCBs blocked on the semaphore */
    struct list_head s_procQ;
} semd_t;

#endif
