#include "../include/system_const.h"
#include "../process/scheduler.h"
#include "../process/pcb.h"
#include "interrupt.h"

// Because the interrupt strat from 8th bit, to look at the second line you must look the 10th
 unsigned int getIPline(unsigned int cause, unsigned int line) {
    line += OFFSET_INT;
    return (cause & (1 << line));
}

void interrupt_Handler(void) {
    state_t *oldAreaInt = (state_t *) OLD_AREA_INTERRUPT;
     
     if (getIPline(oldAreaInt->cause, INTERVAL_TIMER)) {
        // Set the timer to a value, to the timer this is like an ACK
        setTIMER(TIME_SCALE * 10000);
        // Calls the scheduler to select a new process
        scheduler();
     }
}



