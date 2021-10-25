# dmp_kernel.c in /usr/src/servers/is

Add a function in the end:

```c
void procnum_dmp()
{
  register struct proc *rp;
  int r,n=0;
    
  /* First obtain a fresh copy of the current process table. */
  if ((r = sys_getproctab(proc)) != OK) {
    printf("IS: warning: couldn't get copy of process table: %d\n", r);
    return;
  }
    
  for (rp=BEG_PROC_ADDR; rp<END_PROC_ADDR; rp++) {
      if (isemptyp(rp))
        continue;
      n++;
  }
    
  printf("Number of currently running processes is %d\n", n);
}
```

Debugging dump procedures for the kernel.