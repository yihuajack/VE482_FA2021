#include "syslib.h"

/*===========================================================================*
 *                                sys_chsched			     	     *
 *===========================================================================*/
int sys_chsched(flag)
        int flag;
{
    message m;
    m.m1_i1 = flag;
    return (_kernel_call(SYS_CHSCHED, &m));
}

