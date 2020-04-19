#ifndef __TRAP_H
#define __TRAP_H

#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "syscall_breakpoint.h"
#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "tlb.h"


void trap_handler(void);

#endif