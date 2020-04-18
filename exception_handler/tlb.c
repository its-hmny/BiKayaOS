#include "../include/system_const.h"

state_t *old_Area = NULL;

void tlb_handler(void) {
    old_Area = OLD_AREA_TLB;
    PANIC();
}