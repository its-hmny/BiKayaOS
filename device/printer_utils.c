#include "../include/system.h"
#include "term_utils.h"
#include "printer_utils.h"

/*This file contains all of the function related to printers devices*/

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

static dtpreg_t *printer0_reg = (dtpreg_t *) DEV_REG_ADDR(IL_PRINTER, 0);

/*Returns the status written in the registrer with the opportune bit mask*/
static unsigned int dev_status(dtpreg_t *dev_p) {
    return((dev_p->status) & STATUS_MASK);
}

static unsigned int print_char(char c) {
    unsigned int stat = dev_status(printer0_reg);
    /*Check the if the device is ready/installed before using it*/
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

/*Takes as input a pointer to a string/char array that MUST BE null terminated (\0), else it will go to infinite loop*/
void send_printer (char *buffer) {
    while (1) 
        /*If we have arrived at the null then we print the \n value*/
        if ((*buffer) == 0) {
            print_char('\n');
            return;
        }
        /*This other statement print the next char and check also from possible errors*/
        else if (print_char(*buffer++)) {
            term_puts("ERROR: writing into printer device\n");
            return;
        }           
}