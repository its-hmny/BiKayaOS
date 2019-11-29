#include "termutils.h"
#include "system.h"

int main(void) {
    term_puts("hello, world\n");
    term_puts("This is a test\n");
    term_puts("This is a test");
    term_puts("This is a test\n");

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

