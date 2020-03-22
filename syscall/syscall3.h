#include "../process/asl.h"
#include "../process/pcb.h"
#include "../process/scheduler.h"

void syscall3(pcb_t* pid);
void kill(pcb_t* root);