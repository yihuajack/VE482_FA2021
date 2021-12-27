# Midterm Review

## Chapter 0: Course Information

## Chapter 1: Operating Systems Overview

| Abbreviations | Full Name                                                    |
| ------------- | ------------------------------------------------------------ |
| SPOOL         | Simultaneous Peripheral Operation On Line                    |
| DOS           | Disk Operating System                                        |
| IM            | Instant Message                                              |
| DVD           | Digital Versatile Disc                                       |
| MP3           | MPEG-1 Audio Layer-3                                         |
| PDA           | Personal Digital Assistant                                   |
| HDD           | Hard Disk Drive                                              |
| PSW           | Program Status Word                                          |
| RAM           | Random Access Memory                                         |
| ROM           | Read Only Memory                                             |
| EEPROM        | Electrically Erasable Programmable Read-Only Memory          |
| CMOS          | Complementary Metal–Oxide–Semiconductor                      |
| BIOS          | Basic Input/Output System                                    |
| ISA           | Industry Standard Architecture                               |
| PCI           | Peripheral Component Interconnect / Personal Computer Interface |
| PCI-E/PCIe    | PCI Express                                                  |
| AGP           | Accelerated Graphics Port                                    |
| SCSI          | Small Computer System Interface                              |
| USB           | Universal Serial Bus                                         |
| IDE (disk)    | Integrated Drive Electronics                                 |
| Modem         | Modulator-Demodulator                                        |

Computer: CPU, Memory, Monitor + Video Controller, Keyboard + Keyboard Controller, HDD + Hard Disk Controller + Bus.

Memory: Registers, Cache, Main Memory, Magnetic Disk, Magnetic Tape.

Five major components of an OS: System Calls, Processes, File System, Input-Output (IO), Protection and Security.

Services: Program Execution, I/O Operations, File Systems, Communication, Resource Allocation, Accounting, Error Detection, Protection and Security.

```c
#include <unistd.h>
int execl(const char *pathname, const char *arg, ...
                /* (char  *) NULL */);
int execlp(const char *file, const char *arg, ...
                /* (char  *) NULL */);
int execle(const char *pathname, const char *arg, ...
                /*, (char *) NULL, char *const envp[] */);
int execv(const char *pathname, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[],
                char *const envp[]);
int execve(const char *pathname, char *const argv[],
                  char *const envp[]);
int chdir(const char *path);
int fchdir(int fd);
#include <sys/types.h>
off_t lseek(int fd, off_t offset, int whence);
```

[What are the different versions of exec used for in C and C++? - Stack Overflow](https://stackoverflow.com/questions/5769734/what-are-the-different-versions-of-exec-used-for-in-c-and-c)

> `exec()` family of functions replaces the current process image with a new process image.
> `execve()` executes the program referred to by pathname.  This causes the program that is currently being run by the calling process to be replaced with a new program, with newly initialized stack,  heap,  and  (initialized and uninitialized) data segments.
> `chdir()` changes the current working directory of the calling process to the directory specified in path.
> `fchdir()` is identical to `chdir()`; the only difference is that the directory is given as an open file descriptor.
> `lseek()` repositions the file offset of the open file description associated with the file descriptor `fd` to the argument `offset` according to the directive `whence`.

Busy waiting: Call the Driver, Start the Input-Output, Wait in a Tight Loop, Continuously Poll the Device to Know Its State.

System call (see RC Part 1): the programmatic way in which a computer program requests a service from the kernel of the operating system on which it is executed.

An operating system has three main functions: (1) manage the computer's resources, such as the central processing unit, memory, disk drives, and printers, (2) establish a user interface, and (3) execute and provide services for applications software.

## Chapter 2: Processes and Threads

A *process* is an abstraction of a running program at the core of the OS, the unit for resource management.

A thread consists of a thread ID, the program counter, a register set, a stack space.

All the threads within a process share the same code section, data section, operating system resources.

Why threads?

- Perform operations in parallel on the same data
- Perform operations in parallel on the same data
- Often more efficient than separate processes

User-Level Threads:

- Thread creation, destruction, and scheduling done at user level. To create a thread, must allocate storage for stack, program counter, registers, state, etc, in a *thread table*. When a thread yields the CPU or blocks, the thread library saves everything in the thread table. The thread scheduler finds the highest-priority thread that’s ready to run. Its registers, program counter, etc., are restored from *its* thread table.
- Efficiency Gains: No system calls needed. No context switch. No copying data safely across a protection boundary. No need to flush the hardware memory cache.
- Disadvantage: Blocking system calls. Page faults. Inability to benefit from multiprocessor CPUs. Kernel can’t automatically grow per-thread stacks.

Kernel Threads:

- Similar thread table, but in the kernel
- Often integrated with process table
- No problem with blocking system calls — ordinary process scheduling does the right thing
- Handles multi-CPU case easily
- But — thread-switching is expensive, since it needs a context switch
- Still cheaper than process switches, since you can use the same virtual memory map.

Pop-Up Threads: [OS Pop-up Threads (codescracker.com)](https://codescracker.com/operating-system/pop-up-thread.htm)

- When a message arrives, the kernel creates a new thread
- Sometimes, the thread can run entirely in the kernel
- Messy and delicate — what process should own the thread, what sorts of events result in thread creation, etc.

## Chapter 3: Interprocess Communication

Critical region, Atomicity, Race condition, Mutual Exclusion: See OSTEP 26.4.

Monitor: See [Monitor.md](Monitor.md) and [Monitor (synchronization) - Wikipedia](https://en.wikipedia.org/wiki/Monitor_(synchronization)).

Barrier: See [Barrier.md](Barrier.md) and [Barrier (computer science) - Wikipedia](https://en.wikipedia.org/wiki/Barrier_(computer_science)).

## Chapter 4: Scheduling

[What is Burst time, Arrival time, Exit time, Response time, Waiting time, Turnaround time, and Throughput? (afteracademy.com)](https://afteracademy.com/blog/what-is-burst-arrival-exit-response-waiting-turnaround-time-and-throughput)

Burst time: the total time taken by the process for its execution on the CPU.

Arrival time: the time when a process enters into the ready state and is ready for its execution.

Exit time: the time when a process completes its execution and exit from the system.

Response time: the time spent when the process is in the ready state and gets the CPU for the first time.

Response time = Time at which the process gets the CPU for the first time - Arrival time

Waiting time: the total time spent by the process in the ready state waiting for CPU.

Waiting time = Turnaround time - Burst time

Turnaround time: the total amount of time spent by the process from coming in the ready state for the first time to its completion.

Turnaround time = Burst time + Waiting time = Exit time - Arrival time

Switching process is expensive:

- Switch from user mode to kernel mode
- Save state of current process (save register, memory map, etc.)
- Run scheduling algorithm to select a new process
- Remap the memory address for the new process
- Start new process

Goals when scheduling:

- All systems: Fairness, Balance, Policy enforcement.

- Interactive systems: Response time, Proportionality.

Two-Phase Lock: See OSTEP 28.16.

### Dining Philosophers Problem

See OSTEP 31.6.

### Producers-Consumers Problem

See OSTEP 30.2 and 31.4.

## Chapter 5: Deadlocks

Conditions for Deadlock: See OSTEP 32.3

- Mutual exclusion: Threads claim exclusive control of resources that they require (e.g., a thread grabs a lock).
- Hold-and-wait: Threads hold resources allocated to them (e.g., locks that they have already acquired) while waiting for additional resources (e.g., locks that they wish to acquire).
- No preemption: Resources (e.g., locks) cannot be forcibly removed from threads that are holding them.
- Circular wait: There exists a circular chain of threads such that each thread holds one or more resources (e.g., locks) that are being requested by the next thread in the chain.

Three main recovery strategies of recovering from a deadlock:

- Preemption
- Rollback
- Killing

Deadlock Prevention

### Banker's Algorithm

See [Banker's Algorithm.md](Banker's Algorithm.md).

### Cigarette Smokers Problem

See RC slides.

### Readers-Writers Problem

See OSTEP 31.5 and h5ex5.

### Sleeping Barber Problem

See [Sleeping Barber Problem.md](Sleeping Barber Problem.md).
