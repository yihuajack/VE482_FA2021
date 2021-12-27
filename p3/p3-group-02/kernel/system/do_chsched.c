/* The kernel call implemented in this file:
 *   m_type:	SYS_CHSCHED
 *
 * The parameters for this kernel call are:
 *    m1_i1:    the value of new 'sched_t', int
 */

#include "kernel/system.h"
#include "kernel/vm.h"
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <minix/endpoint.h>
#include <minix/u64.h>

#if USE_CHSCHED


/*===========================================================================*
 *				do_chsched				     *
 *===========================================================================*/
int do_chsched(struct proc * caller, message *m_ptr) {
//    printf("do_chsched from KERNEL: called\n");
    if (m_ptr->m1_i1 == 1) {
        sched_t = (sched_t + 1) % 3;
        switch (sched_t) {
            case 0:
               printf("KERNEL: sched_t changed to default!\n");
               break;
            case 1:
               printf("KERNEL: sched_t changed to lottery!\n");
               break;
            case 2:
                printf("KERNEL: sched_t changed to EDF!\n");
                break;
            default:
                printf("KERNEL: wrong sched_t value.\n");
        }
        printf("KERNEL: sched_t = %d now.\n", sched_t);
    }
//    printf("do_chsched from KERNEL: quit\n");
    return OK;
}

#endif /* USE_CHSCHED */