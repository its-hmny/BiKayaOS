#include "pcb.h"
#include "asl.h"
#include "scheduler.h"
#include "../syscall/syscall3.h"
#include "../include/uARM/uarm/uARMtypes.h"
#include "../include/uARM/uarm/uARMconst.h"
#include "../include/uMPS/umps/cp0.h"
#include "../include/system_const.h"

void syscallHandler();
void syscall3(pcb_t *pid);