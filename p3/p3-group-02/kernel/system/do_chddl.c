/* The kernel call implemented in this file:
 *   m_type:	SYS_CHDDL
 *
 * The parameters for this kernel call are:
 *    m2_i1:	CHDDL_ENDPOINT	(endpt of process whose deadline to be changed)
 *    m2_l2:	CHDDL_VALUE		(value of deadline to be set)
 */

#include "kernel/system.h"
#include "kernel/vm.h"
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <minix/endpoint.h>
#include <minix/u64.h>
#if USE_CHDDL

/*===========================================================================*
 *				do_chddl					     *
 *===========================================================================*/
int do_chddl(struct proc * caller, message * m_ptr)
{
	struct proc *rp;
	rp = proc_addr(m_ptr->m2_i1);
	rp->p_deadline = m_ptr->m2_l2;
	return OK;
}
#endif
