#include "../include/system_const.h"
#include "../include/types_bikaya.h"

state_t *oldarea = NULL;

void trap_handler(void) {
    oldarea = (state_t*) OLD_AREA_TRAP;
    PANIC();
}