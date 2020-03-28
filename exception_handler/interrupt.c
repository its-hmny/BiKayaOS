#include "../include/system_const.h"
#include "../process/scheduler.h"
#include "../process/pcb.h"
#include "interrupt.h"

#define WORDSIZE 4
#define INTERRUPT_CODE 0
#define LINE_MASK 0xFF00
#define LINE_OFFSET 8

state_t *oldArea = NULL;

// Returns the exception code from the cause registrer in the old area
HIDDEN unsigned int getExCode(void) {
    unsigned int causeReg = CAUSE_REG(oldArea);
    return(CAUSE_GET_EXCCODE(causeReg));
}

// Returns pending and non-pending interrupt as a vector
HIDDEN void getInterruptLines(unsigned int interruptVector[]) {
   unsigned int interruptLines = (((CAUSE_REG(oldArea)) & LINE_MASK) >> LINE_OFFSET);

   for (unsigned int i = 0; i < 8; i++)
      interruptVector[i] = interruptLines & (1 << i);
}

HIDDEN void interruptDispatcher(unsigned int int_line) {

}

void interrupt_handler(void) {
   oldArea = (state_t*) OLD_AREA_INTERRUPT;
   
   // In uARM an interrupt can block the current instructon so it has to be broght back th PC
   #ifdef TARGET_UARM
   pcb_t *currentProc = getCurrentProc();
   if (currentProc != NULL)
      currentProc->p_s.pc -= WORDSIZE;
   #endif

   if (getExCode() != INTERRUPT_CODE)
      PANIC();
   
   unsigned int interruptVector[8];
   getInterruptLines(interruptVector);

   for (int line = 0; line < 8; line++)
      if (interruptVector[line] == 1)
         interruptDispatcher(line);
}