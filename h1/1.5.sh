#bin/bash

# 1. Create a new user
user add -m -g users yihua  # "users" can be replaced by "other"
# For version < 3.2.0, use
# adduser yihua other /home/yihua

# 2. List all the currently running processes
ps -x
# -a: Print all processes with controlling terminals
# -l: Give long listing
# -x: Include processes without a terminal
# -E: Print kernel endpoint numbers where pids are normally printed
# '-' is optional

# 3. Display the characteristics of the CPU and the available memory
cat /proc/cpuinfo
cat /proc/meminfo
# For dynamic display, try `top` command

# 4. Redirect some random output into two different files
head -n 1 /dev/urandom | tee 1.txt > 2.txt
# Use /dev/urandom rather than /dev/random ('u' means 'unlimited'), see
# https://unix.stackexchange.com/questions/324209/when-to-use-dev-random-vs-dev-urandom
# https://www.2uo.de/myths-about-urandom/
# The original stream is 2-base
# Adding an `od` pipe before `head` after `tee` can directly convert to other-base digits

# 5. Concatenate the two previous files
cat 1.txt 2.txt > 3.txt

# 6. Read the content of the resulting file as hexdecimal values (in other words find a command to read a file as hexadecimal values)
od -x 3.txt
# Usage: od [-bcdhovx] [file] [ [+] offset [.] [b] ]
# -x: Dump words in hex
# or `hexdump 3.txt`

# 7. Use a single command to find all the files in `/usr/src` with the word `semaphore` in their name and containing the word `ddekit_sem_down`
find -X /usr/src -name "*semaphore*" | xargs grep -lw 'ddekit_sem_down'
# Different from Linux, `grep` of Minix3 cannot accept stdin as arguments, so `xargs` is needed
# Double quotes are recommended for `find` while single quotes are recommended for `grep`
# The `-X` option is a modification to permit `find` to be safely used in conjuction with xargs(1). If a file name contains any of the delimiting characters used by `xargs`, a diagnostic message is displayed on standard error, and the file is skipped.
# Alternatively, the `-print0` or `-printx` primaries can be used to format the output in a way that `xargs` can accept.