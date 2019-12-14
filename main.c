#include "include/termutils.h"
#include "include/devutils.h"
#include "include/system.h"

#define STR_LENGHT         30

int main(void) {
    term_puts("Hello from Phase 0 of the BiKayaOS, please enter a string:\n");
    term_puts("Note that you can write at max 30 letter\n");

    char usr_input[STR_LENGHT];
    term_gets(usr_input, STR_LENGHT);

    /*If an error occured term_gets return an empty string with a '\0' in the first index, same for an empty string*/
    if (usr_input[0])
        print(&usr_input);

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

