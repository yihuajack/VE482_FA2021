# Banker's Algorithm - C Implementation

Author: Yihua Liu

Sample input (take Ex. 2 as example):

```text
3
3 3 2
5
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
```

Sample output:

```text
The number of resource types: The number of available resources of each type as a vector (separate by space): The number of processes: The maximum demand of each process as a matrix (row by process, column by resource):
The number of resources of each type currently allocated to each process as a matrix (row by process, column by resource):
The remaining resource need of each process as a matrix (row by process, column by resource):
7 4 3
1 2 2
6 0 0
0 1 1
4 3 1
Execute Process: 1
Current number of available resources of each type: 5 3 2
Execute Process: 3
Current number of available resources of each type: 7 4 3
Execute Process: 4
Current number of available resources of each type: 7 4 5
Execute Process: 0
Current number of available resources of each type: 7 5 5
Execute Process: 2
Current number of available resources of each type: 10 5 7
One possible sequence of processes that the system is in a safe state (process number starting from 1):
 P2 -> P4 -> P5 -> P1 -> P3
```

Usage:

```bash
clang -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla -fno-omit-frame-pointer -fsanitize=address,undefined,integer -o h5ex3 h5ex3.c && ./h5ex3 < input.txt
```

Input specifications:

| Input data                                                   | Variable  | Type         |
| ------------------------------------------------------------ | --------- | ------------ |
| The number of resource types                                 | n_res     | unsigned int |
| The number of available resources of each type               | *r_avail  | int          |
| The number of processes                                      | n_proc    | unsigned int |
| The maximum demand of each process                           | **m_max   | int          |
| The number of resources of each type allocated to each process | **m_alloc | int          |

