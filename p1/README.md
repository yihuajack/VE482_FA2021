# VE482 Project 1 - Mumsh

`mumsh` is a light-weighted shell similar to `bash`. It has the following features:

- Read, parse, and execute commands by loops
- Handle single commands without arguments
- Support commands with arguments
- File I/O redirection
- Support for bash style redirection syntax
- Pipes
- Support CTRL-D (similar to bash, when there is no/an unfinished command)
- Internal commands: `pwd` (can be piped or redirected) and `cd`
- Support CTRL-C
- Support quotes (including single quotes, double quotes, and incomplete quotes)
- Wait for the command to be completed when encountering >, <, or |
- Handle errors for all supported features
- Background commands (commands ending with an &)

You can build `mumsh` by

```bash
clang -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -o mumsh *.c
```

and check memory by

```bash
clang -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer -o mumsh_memory_check *.c
```

The contents of the codes include:

- `p1.h`: definitions of all global variables, macros, structs, header files, and funtions
- `p1.c`: implementations of all functions