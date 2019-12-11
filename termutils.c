#include "include/system.h"
#include "include/termutils.h"

/*This file contains all of the function related to terminal device*/

/*LIST OF THE POSSIBLE ERROR CODE RETURNED FROM tp->*_status*/
#define TERM_N_INSTALLED   0
#define ST_READY           1
#define ILLEGAL_OPCODE     2
#define ST_BUSY            3
#define RECV_ERR           4
#define TRANSM_ERR         4
#define ST_TRANSMITTED     5
#define ST_RECEIVED        5

/*LIIST OF THE POSSIBLE COMMAND INPUT TO tp->*_command*/
#define RESET              0
#define CMD_ACK            1       /*Interrupt to free the terminal and make it avaiable to other user*/
#define CMD_TRANSMIT       2
#define CMD_RECEIVE        2       /*They both impose to the terminal to make an operation (transmit/receive)*/

#define CHAR_OFFSET        8       /*The data transmitted/received are/shall be placed from (8 to 15 bit) used to shift*/
#define TERM_STATUS_MASK   0xFF    /*0.0.0.11111111 => 255. Used to mask the first 12 bit (most significant one)*/
#define DATA_MASK          0xFF00  /*The mask to clean the data rcv'd => 0.0.11111111.0*/

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

    /*Shift the char to be transmitted of 8 bits and add the transmit command "opcode", the opcode is in the first 8 bit of the
    message but in next 8 bit there has to be the data to be transmitted*/
    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    /*Wait if it's busy (busy waiting!)*/
    while ((stat = trans_status(term0_reg)) == ST_BUSY)
        ;

    /*The acknowledgement makes the terminal avaiable to other once finished*/
    term0_reg->transm_command = CMD_ACK;

    /*Error handler, return error code*/
    if (stat != ST_TRANSMITTED)
        return -1;
    else
        return 0;
}

static int term_getchar() {
    /*Check that ther terminal is ready and there are no error*/
    unsigned int stat = recv_status(term0_reg); int char_read;
    if (stat != ST_READY && stat != ST_RECEIVED)
        return -1;
    
    /*Ask the terminal to receive data*/
    term0_reg->recv_command = (CMD_RECEIVE);

    /*Wait for the terminal to fetch and transfer the data requested*/ 
    while ((stat = recv_status(term0_reg)) == ST_BUSY)
        ;

    /*Once exit from the cicle the data may/may not have been fetched but it's stored anyway some result in char_read, the data
    are in bit 8 to 15 (7 or less is the status bit) so we need to mask the status and then shift the data with an offset*/
    char_read = ((term0_reg->recv_status & DATA_MASK) >> CHAR_OFFSET);

    /*We now "free" the terminal with the ACK interrupt*/
    term0_reg->recv_command = CMD_ACK;

    /*Checks for error and returns error code*/
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
    int i, letter;

    /*Stops at the penultimate index and then puts str terminator at the end (last index)*/
    for (i = 0; i < STR_LENGHT-1; i = i + 1) {
        letter = term_getchar();

        /*Error handler, stops the execution and print an error message*/
        if (letter == -1) {
            term_puts("ERROR: reading from terminal\n");
            usr_input[0] = '\0';
            return;
        }
        /*If the usr has pressed ENTER then stop the execution and return the string*/
        else if ((char)letter == '\n') {
            usr_input[i] = 0;
            return;
        }
        else 
            usr_input[i] = (char)letter;
    }

    /*Terminate the string with the 0, and return the readed string*/
    usr_input[i] = '\0';
}