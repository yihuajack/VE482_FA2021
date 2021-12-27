#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <unistd.h>
#include <sys/time.h>

int chddl(long deadline){
	struct timeval tvalue;
	struct timezone tzone;
	message m;
	
	alarm((unsigned int)deadline);
	if(deadline>0){
    		gettimeofday(&tvalue,&tzone);
    		deadline = tvalue.tv_sec + deadline; 
    	}
	m.m2_l2 = deadline;
	return (_syscall(PM_PROC_NR,PM_CHDDL,&m));    
}