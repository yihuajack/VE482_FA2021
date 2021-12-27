#include "pm.h"
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <minix/callnr.h>
#include <minix/endpoint.h>
#include <minix/com.h>
#include <minix/syslib.h>
#include <minix/vm.h>
#include <minix/ipc.h>
#include <sys/resource.h>
#include "glo.h"
#include "mproc.h"
#include "param.h"

int do_chsch(){
	//sys_clear(m_in.m2_l2);
	sys_chsched(1);
	return OK;
}