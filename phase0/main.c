#include "termutils.h"
#include "system.h"

#define STR_LENGHT         30

int main(void) {
    term_puts("Hello from Phase 0 of the BiKayaOS, please enter a string:\n");

    /*Need something that wait for the user input to be done bedore proceeding??*/

    char usr_input[STR_LENGHT];
    term_gets(usr_input, STR_LENGHT); 

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

