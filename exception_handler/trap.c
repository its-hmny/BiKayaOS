#include "./trap.h"
#include "../include/system_const.h"
#include "../generics/utils.h"

HIDDEN state_t *old_area = NULL;

void trap_handler(void) {
    old_area = (state_t*) OLD_AREA_TRAP;

    loadCustomHandler(TRAP_CUSTOM, old_area);
}