#ifndef __TLB_H
#define __TLB_H

#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "syscall_breakpoint.h"
#include "../include/types_bikaya.h"
#include "../include/system_const.h"
#include "tlb.h"

void tlb_handler(void);

#endif