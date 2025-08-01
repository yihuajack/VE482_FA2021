# Makefile for libsys
.include <bsd.own.mk>

LIB=		sys

.include "arch/${MACHINE_ARCH}/Makefile.inc"

SRCS+=  \
	alloc_util.c \
	assert.c \
	asynsend.c \
	ds.c	\
	env_get_prm.c \
	env_panic.c \
	env_parse.c \
	env_prefix.c \
	fkey_ctl.c \
	getsysinfo.c \
	getuptime.c \
	getuptime2.c \
	input.c \
	kernel_call.c \
	kprintf.c \
	kputc.c \
	kputs.c \
	optset.c \
	panic.c \
	safecopies.c \
	sched_start.c \
	sched_stop.c \
	sef.c \
	sef_init.c \
	sef_liveupdate.c \
	sef_ping.c \
	sef_signal.c \
	sqrt_approx.c \
	stacktrace.c \
	sys_chddl.c \
	sys_chsched.c \
	sys_abort.c \
	sys_clear.c \
	sys_cprof.c \
	sys_endsig.c \
	sys_exec.c \
	sys_exit.c \
	sys_fork.c \
	sys_getinfo.c \
	sys_getsig.c \
	sys_hz.c \
	sys_irqctl.c \
	sys_kill.c \
	sys_mcontext.c \
	sys_memset.c \
	sys_physcopy.c \
	sys_privctl.c \
	sys_profbuf.c \
	sys_runctl.c \
	sys_safecopy.c \
	sys_safememset.c \
	sys_schedctl.c \
	sys_schedule.c \
	sys_setalarm.c \
	sys_setgrant.c \
	sys_sigreturn.c \
	sys_sigsend.c \
	sys_sprof.c \
	sys_statectl.c \
	sys_stime.c \
	sys_sysctl.c \
	sys_times.c \
	sys_trace.c \
	sys_umap.c \
	sys_update.c \
	sys_vircopy.c \
	sys_vmctl.c \
	sys_vsafecopy.c \
	sys_vtimer.c \
	sys_vumap.c \
	taskcall.c \
	tickdelay.c \
	timers.c \
	vm_brk.c \
	vm_exit.c \
	vm_fork.c \
	vm_info.c \
	vm_map_phys.c \
	vm_notify_sig.c \
	vm_umap.c \
	vm_yield_get_block.c \
	vm_procctl.c \
	vprintf.c

.if ${MKPCI} != "no"
SRCS+=	pci_attr_r16.c \
	pci_attr_r32.c \
	pci_attr_r8.c \
	pci_attr_w16.c \
	pci_attr_w32.c \
	pci_attr_w8.c \
	pci_del_acl.c \
	pci_dev_name.c \
	pci_find_dev.c \
	pci_first_dev.c \
	pci_get_bar.c \
	pci_ids.c \
	pci_init.c \
	pci_next_dev.c \
	pci_rescan_bus.c \
	pci_reserve.c \
	pci_set_acl.c \
	pci_slot_name.c
.endif

.if ${MKCOVERAGE} != "no"
SRCS+=	gcov.c \
	sef_gcov.c
CPPFLAGS+= -DUSE_COVERAGE
.endif

.if ${USE_LIVEUPDATE} != "no"
CPPFLAGS+= -DUSE_LIVEUPDATE
.endif

.if ${USE_SYSDEBUG} != "no"
CPPFLAGS+= -DUSE_SYSDEBUG
.endif

CPPFLAGS.sched_start.c+=	-I${NETBSDSRCDIR}

.if (${CC} == "gcc") || (${CC} == "clang")
LDADD+=	-lminc
.endif

.include <bsd.lib.mk>
