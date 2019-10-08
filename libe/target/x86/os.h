/*
 * x86 specific os routines.
 */

#ifndef _TARGET_X86_OS_H_
#define _TARGET_X86_OS_H_

int os_init(void);
void os_quit(void);
#define os_wdt_reset()

#endif /* _TARGET_X86_OS_H_ */
