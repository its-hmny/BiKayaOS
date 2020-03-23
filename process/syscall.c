#include "syscall.h"

state_t *old_area;

//return the exception code of exception
unsigned int getCause (unsigned int cause){
    return (CAUSE_GET_EXCCODE(cause));
}

void syscallHandler(){

old_area = (state_t*)OLD_AREA_SYSCALL;
int exCode = getCause(CAUSE_REG(old_area)); 

if(exCode == SYS_CALL){  //if the exception code is a syscall and not a breakpoint
      int numberOfSyscall = a0(old_area);

        switch (numberOfSyscall) {
            case 1:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 2:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 3:
                syscall3(a1(old_area)); //terminate current process and all its offspring (progenie), remove them from the ready queue
            break; 

            case 4:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 5:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 6:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 7:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 8:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 9:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;

            case 10:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break; 

            case 11:
                termprint("I catched an exception, I'm the handler \n");
                PANIC();
            break;           
}


}

}
