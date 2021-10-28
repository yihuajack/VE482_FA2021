# l5ex2

The algorithm of merge sort is included in `algorithm.h`,`algorithm.c`.

The data structure of `dlist` and corresponding functions are included in `lab5_dlist.h`,`lab5_dlist.c`.

`Makefile` is provided by JOJ.

`CMakeLists.txt` is only used for debugging.

`main.c` is based on the incomplete code segment of `main.c` on JOJ, which may be slightly different from JOJ's version.

Known issue: `getValueFromLine()` is different from JOJ's version, which will not allocate memory for `value->strValue`. To pass JOJ, this version will cause memory leak of 10,000 blocks because of `value->strValue` are allocated by `malloc` but not freed.