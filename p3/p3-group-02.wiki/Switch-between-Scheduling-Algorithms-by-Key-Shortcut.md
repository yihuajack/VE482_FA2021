# Switch between Scheduling Algorithms by Key Shortcut
We implement the key shortcut to switch among scheduling algorithms, aka among **Default**, **Lottery**, and **EDF** by pressing `Shift+F7`. Briefly speaking, this feature can be divided into two main parts: a) process the keyboard input and b) implement a system call.

## Scheduling Algorithm Switching
Since the way of switching among algorithms is not specified in manuel, we propose such a method: when `Shift+F7` is pressed in Minix, the scheduling algorithm will be switched by a map, introduced below:
```
when press Shift+F7 : Algorithm |-> Algorithm

        default -> lottery
        lottery -> EDF
        EDF -> Default
```

## Keyboard Input Processing
Change files under `servers/is/` to add one another keyboard shortcut. 

1. In `servers/is/dmp.c` find the `hook_entry` and add the new shortcut you need, bound with the function.
```c
    { SF7, chsched_dmp, "Switch schedule method"},

```
2. Add the declaration of function in `proto.h`, and implement the function in `servers/is/dmp_kernel.c`:

```c
/*===========================================================================*
 *				chsched_dmp				     *
 *===========================================================================*/
void chsched_dmp() {
    message m;
    m.m2_i1 = 1;
    _syscall(PM_PROC_NR,PM_CHSCH,&m); 
    printf("myKEYBOARD: Changed sched_t.\n\n");
}


```

## System Call
We have seen the keyboard shortcut handler `chsched_dmp` sends a message to `PM`. This is because `IS` has no privilege to use system calls. So we decide to inform `PM` to do this. In `include/minix/callnr.h` define the system call macro
```c
#define PM_CHSCH          70
```
and connect it through `servers/pm/table.c`
```c
	do_chsch,	/* 70 = chsch	*/
```
with the implementation in `servers/pm/chsch.c`
```c
int do_chsch(){
	sys_chsched(1);
	return OK;
}
```
calling the system call `int sys_chsched(int flag)`. This is defined in `include/minix/syslib.h`, and implemented in `lib/libsys/sys_chsched.c`.


```c
int sys_chsched(flag)
        int flag;
{
    message m;
    m.m1_i1 = flag;
    return (_kernel_call(SYS_CHSCHED, &m));
}
```
It use `_kernel_call` to trap into kernel, and called `do_chsched` implemented in `kernel/system/do_chsched.c` (with the binding ` { "CHSCHED",    SYS_CHSCHED },` and `  map(SYS_CHSCHED, do_chsched);` in `commands/service/parse.c` and `kernel/system.c` respectively):
```c
/*===========================================================================*
 *				do_chsched				     *
 *===========================================================================*/
int do_chsched(struct proc * caller, message *m_ptr) {
    if (m_ptr->m1_i1 == 1) {
        sched_t = (sched_t + 1) % 3;
        printf("do_chsched from KERNEL: sched_t changed to %d!\n", sched_t);
    }
    return OK;
}
```
When `flag` is zero, it will increment scheduling algorithm indicator `int sched_t` (with `mod 3`).





