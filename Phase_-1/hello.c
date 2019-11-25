#include "termprint.h"
#include "system.h"

int main(void)
{
    term_puts("hello, world\n");

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

