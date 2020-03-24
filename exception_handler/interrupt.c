#include "../include/system_const.h"
#include "../process/scheduler.h"
#include "../process/asl.h"
#include "../process/pcb.h"
// #include "interrupt.h"


 unsigned int getIPline(unsigned int cause, int line){
    //perchè gli interrupt iniziano dall'8 bit in poi. quindi se vuoi vedere sulla linea 2 devi vedere il 2+8=10imo bit
    line += OFFSET_INT;
    return cause & (1 << line);
}

void interruptHandler(){
 
    state_t *oldAreaInt = (state_t *) OLD_AREA_INTERRUPT;
     
     if(getIPline(oldAreaInt->cause, INTERVAL_TIMER))
        intervalTimer();
}

void  intervalTimer(){
    //ACK set the timer at random value
    setTIMER(TIME_SCALE * 10000);
    scheduler();
    }
