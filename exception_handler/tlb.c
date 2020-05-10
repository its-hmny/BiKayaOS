#include "./tlb.h"
#include "../include/system_const.h"
#include "../generics/utils.h"

HIDDEN state_t *old_area = NULL;

void tlb_handler(void) {
    old_area = (state_t*) OLD_AREA_TLB;
    
    loadCustomHandler(TLB_CUSTOM, old_area);
}