#ifdef TARGET_UMPS
#include "uMPS/libumps.h"
#include "uMPS/arch.h"
#include "uMPS/types.h"
#include "uMPS/umps/cp0.h"
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

#define DEFAULT_PRIORITY 1

#define DEV_PER_INT 8 // Maximum number of devices per interrupt line
#define MAX_LINE 8
#define WORDSIZE 4
#define CR 0x0a   // Carriage return as returned by the terminal

#define TIME 3000
#define TIME_SLICE (TIME * TIME_SCALE)

#ifndef NULL
#define NULL ((void *) 0)
#define OFFSET_INT 8
#endif

/* ======== CONSTANTS FOR STATE_T OPTION/PARAMETERS ============= */
#ifdef TARGET_UMPS

// Status registrer bits for enabling/disabling interrupts in the given process
#define DISABLE_INTERRUPT    0
#define ENABLE_INTERRUPT     1 
#define IEP_SHIFT 2
#define INTERRUPT_MASK_SHIFT 8

// Interrupt bitmask (only for uMPS ignored by uARM)
#define ALL_INTRRPT_ENABLED 0xFF
#define ALL_INTRRPT_DISABLED 0x00
#define ONLY_TIMER_ENABLED 0x04

// Status registrer bits for enabling/disabling kernel mode in the given process
#define KERNEL_MD_ON    0
#define USR_MD_ON       1
#define KM_SHIFT        1

// Status registrer bits for enabling/disabling virtual memory in the given process
#define VIRT_MEM_ON      1
#define VIRT_MEM_OFF     0
#define VIRT_MEM_SHIFT   24

// Status registrer bits for enabling/disabling timer in the given process
        // In uMPS this sets the PLT that is not used (always off)
#define PLT_DISABLED  0
#define PLT_SHIFT     27

#endif

#ifdef TARGET_UARM
// Option to disable or enable all interrupt lines (is not possible to activite only some as in uMPS)
#define DISABLE_INTERRUPT 0
#define ENABLE_INTERRUPT  1

// Status registrer bits for enabling/disabling kernel mode in the given process
#define KERNEL_MD_ON 1
#define USR_MD_ON    0

// Status registrer bits for enabling/disabling virtual memory in the given process
#define VIRT_MEM_ON      1
#define VIRT_MEM_OFF     0

// Status registrer bits for enabling/disabling Interval Timer in the given process
#define TIMER_ENABLED  1
#define TIMER_DISABLED 0

#endif

/* ==================== OLD/NEW AREAS AND RRF address =========================== */
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
#define _RAMTOP     (RAMBASE + RAMSIZE)
#define RAM_FRAMESIZE  4096

//Time areas 
#define INTERVAL_TIMER BUS_REG_TIMER
#define TIME_SCALE     *((unsigned int *)BUS_REG_TIME_SCALE)
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
#define _RAMTOP RAM_TOP
#define RAM_FRAMESIZE FRAME_SIZE

// Time areas
#define INTERAVAL_TIMER 0x000002E4
#define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
#endif

/* ======================== EXCEPTION HANDLING MACROS ============================== */
#ifdef TARGET_UMPS
// Get the exception code from the cause register
#define CAUSE_GET_EXCCODE(x)    (((x) & CAUSE_EXCCODE_MASK) >> CAUSE_EXCCODE_BIT)

#define SYSCALL_CODE            8
#define BREAKPOINT_CODE         9
#define INTERRUPT_CODE 0

#define EXC_HANDLER_PROC_OPT { DISABLE_INTERRUPT, KERNEL_MD_ON, ALL_INTRRPT_DISABLED, VIRT_MEM_OFF, PLT_DISABLED }

#endif

#ifdef TARGET_UARM
// Get the exception code from the cause register
#define CAUSE_GET_EXCCODE(x)    ((x) & 0xFFFFFF)

#define SYSCALL_CODE            SYSEXCEPTION
#define BREAKPOINT_CODE         BPEXCEPTION
#define INTERRUPT_CODE INTEXCEPTION

#define EXC_HANDLER_PROC_OPT { DISABLE_INTERRUPT, KERNEL_MD_ON, VIRT_MEM_OFF, TIMER_DISABLED }

#endif

/* ========================= INTERRUPT HANDLING MACROS =============================== */
#ifdef TARGET_UMPS

#define LINE_MASK 0xFF00
#define LINE_OFFSET 8

// Code for each interrupts line
#define INTER_PROCESSOR 0
#define PROCESSOR_LOCAL_TIMER 1
#define BUS_INTERVAL_TIMER 2
#define DISK_DEVICE 3
#define TAPE_DEVICE 4
#define NETWORK_DEVICE 5
#define PRINTER_DEVICE 6
#define TERMINAL_DEVICE 7

#endif

#ifdef TARGET_UARM

// Code for each interrupts line (0,1,5 are not used in uARM)
#define INTER_PROCESSOR 0
#define PROCESSOR_LOCAL_TIMER 1
#define BUS_INTERVAL_TIMER INT_TIMER
#define DISK_DEVICE INT_DISK
#define TAPE_DEVICE INT_TAPE
#define NETWORK_DEVICE INT_UNUSED
#define PRINTER_DEVICE INT_PRINTER
#define TERMINAL_DEVICE INT_TERMINAL

#endif

/* ========================= MACROS FOR REGISTER ACCESS =============================== */
#ifdef TARGET_UMPS

#define STATUS_REG(state) state->status
#define PC_REG(state)     state->pc_epc
#define SP_REG(state)     state->reg_sp
#define CAUSE_REG(state)  state->cause
#define GET_A0_REG(state) state->gpr[3]

#endif

#ifdef TARGET_UARM

#define STATUS_REG(state) state->cpsr
#define PC_REG(state)     state->pc
#define SP_REG(state)     state->sp
#define CAUSE_REG(state)  state->CP15_Cause
#define GET_A0_REG(state) state->a1


#endif

/* ========================= MACROS FOR SYSCALLS =============================== */
#define GETCPUTIME       1
#define CREATEPROCESS    2
#define TERMINATEPROCESS 3
#define VERHOGEN         4
#define PASSEREN         5
#define WAITIO           6
#define SPECPASSUP       7
#define GETPID           8