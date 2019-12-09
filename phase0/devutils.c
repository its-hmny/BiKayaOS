#include "include/system.h"
#include "include/termutils.h"

/*This file contains all of the function related to general device (printers, tapes, disk, etc.)*/

/*LIST OF POSSIBLE ERROR CODE RETURNED BY THE STATUS REGISTRER*/
#define DEV_N_INSTALLED    0
#define DEV_READY          1
#define ILLEGAL_OPCODE     2
#define DEV_BUSY           3
#define PRINT_ERR          4

/*LIIST OF THE POSSIBLE COMMAND INPUT TO dev_p->command registrer*/
#define RESET              0
#define CMD_ACK            1
#define PRINT_CHR          2

#define STATUS_MASK        0xFF
#define DATA_MASK          0xFF

static dtpreg_t *printer0_reg = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER, 0); /*THE ERROR IS HERE WTF*/

static unsigned int dev_status(dtpreg_t *dev_p) {
    return((dev_p->status) & STATUS_MASK);
}

static unsigned int print_char(char c) {
    unsigned int stat = dev_status(printer0_reg);
    if (stat != DEV_READY || stat == DEV_N_INSTALLED)
        return -1;

    /*Insert the char to be printed in the corresponding registrer*/
    printer0_reg->data0 = c;

    /*Then it sends to the device the PRINT command*/
    printer0_reg->command = PRINT_CHR;
    
    /*Wait for the device to execute*/
    while((stat = dev_status(printer0_reg)) == DEV_BUSY)
        ;

    /*After the operation it frees the device that can now be used by other*/
    printer0_reg->command = CMD_ACK;

    /*Error handler, error code returned*/
    if(stat != DEV_READY)
        return -1;
    else 
        return 0;
}

/*Takes as input a pointer to a string/char array that MUST BE null terminated (\0)*/
void print (char *buffer) {
    while (*buffer)
        /*If we have arrived at the null then we print the \n value*/
        if ((*buffer + 1) == '\0') {
            print_char('\n');
            return;
        }
        /*This other statement print the next char and check also from possible errors*/
        else if (print_char(*buffer++)) {
            term_puts("ERROR: writing into printer device\n");
            return;
        }           
}

void statFinder (unsigned int stat) {
    if(stat == 0)
        term_puts("0");
    else if(stat == 1)
        term_puts("1");
    else if(stat == 2)
        term_puts("2");
    else if(stat == 3)
        term_puts("3");
    else if(stat == 4)
        term_puts("4");
}