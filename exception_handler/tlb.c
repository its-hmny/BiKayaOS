#include "tlb.h"


state_t *old_Area = NULL;

void tlb_handler(void) {
    old_Area = OLD_AREA_TLB;
//  old_Area->pc_epc += WORDSIZE;
    pcb_t* currentProcess = getCurrentProc();
 //   handler tmp = currentProcess->custom_hndlr;
    int code = (currentProcess->custom_hndlr).has_custom_handler[2];
    if(!code){  //if code == 0 then we don't considerer tlb exception instead  
                //if code == true we must terminate process
        syscallDispatcher(3); 
        
    }

    clonestate(old_Area,(currentProcess->custom_hndlr).syscall_old,sizeof(old_Area));
    LDST((currentProcess->custom_hndlr).tlb_new);
    //PANIC();
}