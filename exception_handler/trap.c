#include "../include/system_const.h"
#include "../include/types_bikaya.h"

state_t *oldarea = NULL;

void trap_handler(void) {
    oldarea = OLD_AREA_TRAP;    
//  old_Area->pc_epc += WORDSIZE;
    pcb_t* currentProcess = getCurrentProc();

    int code = (currentProcess->custom_hndlr).has_custom_handler[2];
    if(!code){  //if code == 0 then we don't considerer tlb exception instead  
                //if code == true we must terminate process
        syscallDispatcher(3); 
        
    }
    //BreakpointTrapHandler()   <---  ???
    clonestate(oldarea,(currentProcess->custom_hndlr).trap_old,sizeof(oldarea));
    LDST((currentProcess->custom_hndlr).trap_new);
   
    PANIC();
}