#include "include/system.h"

#define ST_READY           1
#define ST_BUSY            3
#define ST_TRANSMITTED     5

#define CMD_ACK            1
#define CMD_TRANSMIT       2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF

/*Assignation of the terminal file with the built-in macro*/
static termreg_t *term0_reg = (termreg_t *) DEV_REG_ADDR(IL_TERMINAL, 0);

/*This function return the status of the terminal pointer given in input*/
static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

static int term_putchar(char c) {
    /*Check that terminal is ready, there are no errors*/
    unsigned int stat = tx_status(term0_reg);
    if (stat != ST_READY && stat != ST_TRANSMITTED)
        return -1;

    /*TODO don't know*/
    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    /*Wait if it's busy (busy wainting!)*/
    while ((stat = tx_status(term0_reg)) == ST_BUSY)
        ;

    term0_reg->transm_command = CMD_ACK;

    /*Then if the transmission occured 0 else error code*/
    if (stat != ST_TRANSMITTED)
        return -1;
    else
        return 0;
}


void term_puts(const char *str) {
     /* The guards in this cicle is given by the if, the method usually returns 0 if everything went fine
     else return -1 that is casted to true by C compiler so it runs the then "return" and terminates */
    while (*str)
        if (term_putchar(*str++))
            return;
}
