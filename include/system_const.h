#ifdef TARGET_UMPS
#include "uMPS/libumps.h"
#include "uMPS/arch.h"
#include "uMPS/types.h"
#endif
#ifdef TARGET_UARM
#include "uARM/uarm/libuarm.h"
#include "uARM/uarm/arch.h"
#include "uARM/uarm/uARMtypes.h"
#include "uARM/uarm/uARMconst.h"
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


/* ======== CONSTANTS FOR STATE_T STRUCT AND SUBSEQUENT REGISTER ============= */

// Status registrer bits for enabling/disabling interrupts in the given process
#define DISABLE_INTERRUPT 0
#define ENABLE_INTERRUPT 1 
#define INTERRUPT_MASK_SHIFT 8
// ALL interrupts line enabled
#define ALL_INTRRPT_ENABLED 255 

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


#ifdef TARGET_UMPS
    // Status, program counter and stack poiter register in uMPS
#define STATUS_REG(state) state->status
#define PC_REG(state)     state->pc_epc
#define SP_REG(state)     state->reg_sp
#endif
#ifdef TARGET_UARM
    // Status, program counter and stack poiter register in uARM
#define STATUS_REG(state) state->cpsr
#define PC_REG(state)     state->pc
#define SP_REG(state)     state->sp
#endif

/* ==================== OLD/NEW AREAS AND RAM address =========================== */
#ifdef TARGET_UMPS
    // uMPS New/Old Areas address
#define NEW_AREA_SYSCALL   0x200003d4
#define OLD_AREA_SYSCALL   0x20000348
#define NEW_AREA_TRAP      0x200002bc
#define OLD_AREA_TRAP      0x20000230
#define NEW_AREA_TLB       0x200001a4
#define OLD_AREA_TLB       0x20000118
#define NEW_AREA_INTERRUPT 0x2000008c
#define OLD_AREA_INTERRUPT 0x20000000
    // uMPS's beginning address of RAM and size of a RAM page
#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAMTOP     (RAMBASE + RAMSIZE)
#define RAM_FRAMESIZE  4096
#endif

#ifdef TARGET_UARM
    // uARM New/Old Areas address
#define NEW_AREA_SYSCALL   SYSBK_NEWAREA 
#define OLD_AREA_SYSCALL   SYSBK_OLDAREA 
#define NEW_AREA_TRAP      PGMTRAP_NEWAREA
#define OLD_AREA_TRAP      PGMTRAP_OLDAREA
#define NEW_AREA_TLB       TLB_NEWAREA
#define OLD_AREA_TLB       TLB_OLDAREA
#define NEW_AREA_INTERRUPT INT_NEWAREA
#define OLD_AREA_INTERRUPT INT_OLDAREA
    // uARM's beginning address of RAM and size of a RAM page
#define RAMTOP RAM_TOP
#define RAM_FRAMESIZE FRAME_SIZE
#endif