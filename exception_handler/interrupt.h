#ifndef __INTERRUPT_H
#define __INTERRUPT_H

void interrupt_handler(void);

#endif

#define TERM_STATUS_MASK 0xFF
#define TRANSM_STATUS(tp) ((tp->transm_status) & TERM_STATUS_MASK)
#define RECV_STATUS(tp) ((tp->recv_status) & TERM_STATUS_MASK)
#define CMD_ACK 1
#define TERM_SUCCESS 5