#include "../include/system_const.h"
#include "../generics/utils.h"
#include "../process/scheduler.h"
#include "../process/pcb.h"
#include "interrupt.h"

#define WORDSIZE 4
#define INTERRUPT_CODE 0
#define LINE_MASK 0xFF00
#define LINE_OFFSET 8


/* ============= SUBHANDLER DEFINITION ============ */
HIDDEN void tmp(void) {
   PANIC();
}

HIDDEN void intervalTimer_hadler(void) {
   // Send an Ack to the timer, sets him up to a timeslice
   setIntervalTimer();
   // The scheduler will chose a process and reset a timeslice, else it will loop
   scheduler();
}

HIDDEN void terminal_handler(void) {
   //Ignores it, the writer process will send the ack
}



/* ========== INTERRUPT HANDLER ========== */
// Old Area pointer, used to retrieve info about the exception
state_t *oldArea = NULL;
// Vector of subhandler, there's one handler for each interrupt line
void (*subhandler[])(void) = { tmp, tmp, intervalTimer_hadler, tmp, tmp, tmp, tmp, terminal_handler };

// Returns pending and non-pending interrupt as a vector
HIDDEN void getInterruptLines(unsigned int interruptVector[]) {
   #ifdef TARGET_UMPS
   unsigned int interruptLines = (((CAUSE_REG(oldArea)) & LINE_MASK) >> LINE_OFFSET);
   for (unsigned int i = INTER_PROCESSOR; i <= TERMINAL_DEVICE; i++)
      interruptVector[i] = interruptLines & (1 << i);
   #endif

   #ifdef TARGET_UARM
   unsigned int causeReg = CAUSE_REG(oldArea);
   for (unsigned int i = INTER_PROCESSOR; i <= TERMINAL_DEVICE; i++) {
      interruptVector[i] = CAUSE_IP_GET(causeReg, i);
   }
   #endif
}

void interrupt_handler(void) {
   oldArea = (state_t*) OLD_AREA_INTERRUPT;
   
   // In uARM an interrupt can block the current instructon so it has to be broght back th PC
   #ifdef TARGET_UARM
   pcb_t *currentProc = getCurrentProc();
   if (currentProc != NULL)
      currentProc->p_s.pc -= WORDSIZE;
   #endif

   //Trovare l'interrupt code per uARM e aggiungerlo
   //if (getExCode(oldArea) != INTERRUPT_CODE)
      //PANIC();
   
   unsigned int interruptVector[8];
   getInterruptLines(interruptVector);

   for (int line = INTER_PROCESSOR; line <= TERMINAL_DEVICE; line++) {
      if (interruptVector[line] == 1) {
         subhandler[line]();
      }
   }
}