#include "syslib.h"

/*===========================================================================*
 *                                sys_chddl			     	     *
 *===========================================================================*/
int sys_chddl(proc_ep, deadline)
endpoint_t proc_ep;
long deadline; 
{
	message m;
	m.m2_i1 = proc_ep;
	m.m2_l2 = deadline;
	return (_kernel_call(SYS_CHDDL, &m));
}
