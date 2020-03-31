#include "../include/system_const.h"
#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "../process/pcb.h"
#include "interrupt.h"

#define TIME 3000
#define TIME_SLICE (TIME * TIME_SCALE)

// Old Area pointer, used to retrieve info about the exception
state_t *oldArea = NULL;
unsigned int elapsedTime = 0;

/* ============= SUBHANDLER DEFINITION ============ */
HIDDEN void tmp(void) {
   PANIC();
}

HIDDEN void intervalTimer_hadler(void) {
   // Save the current old area state to the process that has executed
   pcb_t *currentProcess = getCurrentProc();
   cloneState(&currentProcess->p_s, oldArea, sizeof(state_t));
   // Send an Ack to the timer, sets him up to a timeslice
   setIntervalTimer();
   // The scheduler will chose a process and reset a timeslice, else it will loop
   scheduler();
}

// Ignores it, the writer process will send the ack
HIDDEN void terminal_handler(void) {
   // Set the timer to the remaining time of execution
   unsigned int remainingTime = ((unsigned int)TIME_SLICE) - elapsedTime;
   setTimerTo(remainingTime);
   //LDST(oldArea); TODO SEE IF NEEDED
}



/* ========== INTERRUPT HANDLER ========== */
// Vector of subhandler, there's one handler for each interrupt line
void (*subhandler[])(void) = { tmp, tmp, intervalTimer_hadler, tmp, tmp, tmp, tmp, tmp };

// Returns pending and non-pending interrupt as a vector
HIDDEN void getInterruptLines(unsigned int interruptVector[]) {
   #ifdef TARGET_UMPS
   unsigned int interruptLines = (((CAUSE_REG(oldArea)) & LINE_MASK) >> LINE_OFFSET);
   for (unsigned int i = 0; i < MAX_LINE; i++)
      interruptVector[i] = interruptLines & (1 << i);
   #endif

   #ifdef TARGET_UARM
   unsigned int causeReg = CAUSE_REG(oldArea);
   for (unsigned int i = 0; i < MAX_LINE; i++) {
      interruptVector[i] = CAUSE_IP_GET(causeReg, i);
   }
   #endif
}

void interrupt_handler(void) {
   elapsedTime = getIntervalTimer();
   oldArea = (state_t*) OLD_AREA_INTERRUPT;
   
   // In uARM an interrupt can block the current instructon so it has to be broght back th PC
   #ifdef TARGET_UARM
   PC_REG(oldArea) = PC_REG(oldArea) - WORDSIZE;
   #endif
   
   // Check the exception code
   if (getExCode(oldArea) != INTERRUPT_CODE)
      PANIC();
   
   // Retrieve a vectorized version of the pending interrupt 
   unsigned int interruptVector[MAX_LINE];
   getInterruptLines(interruptVector);

   for (unsigned int line = 0; line < MAX_LINE; line++) 
      if (interruptVector[line])
         subhandler[line]();
}