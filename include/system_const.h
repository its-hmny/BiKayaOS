#ifdef TARGET_UMPS
#include "uMPS/libumps.h"
#include "uMPS/arch.h"
#include "uMPS/types.h"
#endif
#ifdef TARGET_UARM
#include "uARM/uarm/libuarm.h"
#include "uARM/uarm/arch.h"
#include "uARM/uarm/uARMtypes.h"
#endif

/**************************************************************************** 
 *
 * This header file contains the global constant & macro definitions
 * As well as target architechture includes for both uARM and uMPS
 * 
 ****************************************************************************/

/*  GENERIC GLOBAL CONSTANT  */
#define MAXPROC 20  // Max number of overall (eg, system, daemons, user) concurrent processes 
#define UPROCMAX 3  // Number of usermode processes (not including master proc and system daemons

#define	HIDDEN static
#define	TRUE 	1
#define	FALSE	0
#define ON 	    1
#define OFF 	0
#define EOS   '\0'

#define DEV_PER_INT 8 // Maximum number of devices per interrupt line

#define CR 0x0a   // Carriage return as returned by the terminal


/*  CONSTANT FOR STATE_T STRUCT AND SUBSEQUENT REGISTER  */

// Status registrer bits for enabling/disabling interrupts in the given process
#define DISABLE_INTERRUPT 0
#define ENABLE_INTERRUPT 1 
#define INTERRUPT_MASK_SHIFT 8

// Status registrer bits for enabling/disabling kernel mode in the given process
#define KERNEL_MD_ON 0
#define USR_MD_ON 1
#define KM_SHIFT 1

#define PO_MASK_SHIFT 2

#define VIRT_MEM_ON 1
#define VIRT_MEM_OFF 0
#define VIRT_MEM_SHIFT 24

#define VIRTMEM_PO_SHIFT 25

#define TIMER_ENABLED 1
#define TIMER_DISABLED 0
#define TIMER_SHIFT 27

// The last 30 word of the state_t struct, that are auxiliary registrers (intermediate values, etc)
#define GPR_LENGTH 30