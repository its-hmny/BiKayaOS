#include "include/system.h"

#define ST_READY           1
#define ST_BUSY            3
#define ERROR              4
#define ST_TRANSMITTED     5
#define ST_RECEIVED        5

#define RESET              0
#define CMD_ACK            1
#define CMD_TRANSMIT       2
#define CMD_RECEIVE        2

#define DATA_MASK          0xFF00 /*The mask to clean the data rcv'd => 11111111 00000000*/

#define CHAR_OFFSET        8    /*The data transmitted must be from 8th bit to 15th*/
#define TERM_STATUS_MASK   0xFF /*00000000 00000000 00000000 11111111 => 255*/

/*Assignation of the terminal file with the built-in macro*/
static termreg_t *term0_reg = (termreg_t *) DEV_REG_ADDR(IL_TERMINAL, 0);

/*This function return the status of the terminal pointer given in input*/
static unsigned int trans_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

/*Return staus of the terminal pointer for receiving pourpose*/
static unsigned int recv_status(termreg_t *tp) {
    return((tp->recv_status) & TERM_STATUS_MASK);
} 

static int term_putchar(char c) {
    /*Check that terminal is ready, there are no errors*/
    unsigned int stat = trans_status(term0_reg);
    if (stat != ST_READY && stat != ST_TRANSMITTED)
        return -1;

    /*Shift the char to be transmitted of 8 bits and add the transmit command "opcode"*/
    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    /*Wait if it's busy (busy waiting!)*/
    while ((stat = trans_status(term0_reg)) == ST_BUSY)
        ;

    term0_reg->transm_command = CMD_ACK;

    if (stat != ST_TRANSMITTED)
        return -1;
    else
        return 0;
}

static int term_getchar() {
    /*Check that ther terminal is ready and there are no error after the reset*/
    int char_read;
    unsigned int stat = recv_status(term0_reg);
    if (stat != ST_READY && stat != ST_RECEIVED)
        return -1;
    
    /*Ask the terminal to receive data*/
    term0_reg->recv_command = (CMD_RECEIVE);

    while ((stat = recv_status(term0_reg) == ST_BUSY))
        char_read = ((term0_reg->recv_status & DATA_MASK) >> CHAR_OFFSET);
    term0_reg->recv_command = CMD_ACK;

    /*THE ERROR IS HERE, THE EXECUTION STOPPED HERE!!*/
    if (stat != ST_RECEIVED) 
        return -1;
    else
        return (char_read);
}

void term_puts(const char *str) {
    /* The guards in this cicle is given by the if, the method usually returns 0 if everything went fine
     else return -1 that is casted to true by C cso it runs the then "returns" and terminates */
    while (*str)
        if (term_putchar(*str++))
            return;
}

/*First step to read a string from stdin of the terminal and return a pointer*/
void term_gets(char usr_input[], unsigned int STR_LENGHT) {
    int i;

    /*Stops at the last - 1 index and then puts str terminator at the end*/
    for (i = 0; i < STR_LENGHT-1; i = i + 1) {
        int letter = term_getchar();

        /*Error handler, stops the execution*/
        if (letter == -1) {
            term_puts("ERROR: reading from terminal\n");
            usr_input[0] = '\0';
            return;
        }

        else 
            usr_input[i] = (char)letter;
    }

    /*Terminate the string with the 0, and return the readed string*/
    usr_input[STR_LENGHT - 1] = '\0';

    /*TODO REMOVE for double check pourpose*/
    term_puts(usr_input);
}
