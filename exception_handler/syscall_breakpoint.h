#ifndef __SYSCALL_BREASKPOINT_H
#define __SYCALL_BREAKPOINT_H

void syscall_breakpoint_handler(void);
void syscallDispatcher(unsigned int sysNumber);

#endif