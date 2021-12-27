# EDF Scheduling Implementation
In our implementation of EDF scheduling algorithm, several parts are revised.

## Kernel Space

In kernel space, we revise several parts:
- `/usr/src/kernel/proc.h`
- `/usr/src/kernel/proc.c`
- `/usr/src/kernel/system/do_chddl.c`
- `/usr/src/kernel/system.c/system_init(void)`

### `proc.h`

An extra member variable `long p_deadline` is added to the structure `proc` to record the deadline of each process. By default, the value of `p_deadline` of a process should be 0 if it does not have a deadline.

### `proc.c`

In this file, several functions are changed to support EDF scheduling includes:
- `proc_init(void)`
- `enqueue(register struct proc *rp)`
- `enqueue_head(struct proc *rp)`
- `pick_proc(void)`

In function `proc_init(void)`, the deadline of each process is initialized to 0.

Changes to `enqueue(register struct proc *rp)` and `enqueue_head(struct proc *rp)` are actually identical. Both functions now will check the system environment parameter `sched_t` and the deadline of current process `rp->p_deadline`. If the system is now using the EDF scheduling algorithm and the current process has a deadline greater than 0, then the current process will be put into `queue[USER_Q]`. The priority of the current process will also be changed to `USER_Q`. Otherwise, these two functions will work normally.

The original `pick_proc(void)` function is modified to support the EDF scheduling algorithm. If the system environment parameter `sched_t` equals to 2, `pick_proc(void)` will choose the next process to run according to EDF scheduling algorithm. In that case, this function will check the processes with priority greater than or equal to `USER_Q` (since the EDF algorithm should not block the system processes). For queue corresponding to priority `USER_Q`, this function will check all the processes in this queue since the processes with deadline greater than 0 will be added into this queue. If this queue is not empty, this function will return the process with the smallest value of `p_deadline` if there is any process with non-zero deadline, or the first process in this queue if no process with non-zero deadline. Otherwise, this function will check other queues normally and return the process in the head of the first non-empty queue.

### `do_chddl.c`

This file implements a kernel call `do_chddl(struct proc *caller, message *m_ptr)`, which will change the deadline of the corresponding process.

### `system.c`

In this file, the function `system_init(void)` is modified to create the mapping between the system call `sys_chddl()` and the kernel call `do_chddl(struct proc *caller, message *m_ptr)`.

## User Space

In user space, several parts are modified to implement a new system call `chddl()`
- `/usr/src/servers/pm/chddl.c`
- `/usr/src/servers/pm/proto.h`
- `/usr/src/servers/pm/table.c`
- `/usr/src/servers/pm/Makefile`
- `/usr/src/include/minix/callnr.h`
- `/usr/src/include/minix/com.h`

### `chddl.c`

This file implement the `do_chddl()` function in user space.

### `proto.h`

This file is revised to declare the `do_chddl()` function.

### `table.c`

This file is revised to register a new system call into the table.

### `Makefile`

This file is revised to include `chddl.c` so that the system call can also be compiled when building the system.

### `callnr.h`

This file is revised to register the number of a new system call.

### `com.h`

This file is revised to register the system call number for the call vector.