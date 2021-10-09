#ifndef P1_H
#define P1_H

#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define CMD_MAX_LEN 1025
// stdio_lim.h defines FILENAME_MAX 4096, but FN_MAX_SIZE < CMD_MAX_SIZE
#define FN_MAX_LEN 1024
#define PIPE_MAX_NUM 512
/* For "type.h", isspace: space (0x20), form feed (0x0c), line feed (0x0a), carriage return (0x0d),
 * horizontal tab (0x09) or vertical tab (0x0b), isblank: space (0x20) and horizontal tab (0x09),
 * but p1_specification only specifies ' ', '\t', '\n'.  */
#define TOK_DELIM " \t\n"

bool f_i;  // flag of input redirection
bool f_osingle;  // flag of output direction by overwriting a file
bool f_odouble;  // flag of output direction by appending to a file
char ifn[FN_MAX_LEN], ofn[FN_MAX_LEN];  // input file name and output file name
char *p_path;  // previous path
char **cmds;  // list of commands
int sig_stat;  // 0 for normal, 1 for parent, 2 for child.
int **pipe_fds;  // list of pipes
struct sigaction info, newhandler;  // current action and previous action
struct wd c_wd, *p_wd;  // current working directory and previous working directory
unsigned int cmdbodies[PIPE_MAX_NUM] = {0};  // indices of command bodies excluding arguments
unsigned int n_word;  // number of words in one command line, including NULL delimiters
unsigned int n_cmd;  // number of commands in one command line
unsigned int n_pipe;  // number of pipes in one command line

struct wd{
    char *path;
    unsigned int path_len;
};

bool parse(char *cmdln);
bool quote_check(char *ln);
char *strcpbrk(const char *s1, const char *s2);
char *strqtok(char *parsedln, const bool p_quote[], size_t *p_tok);
char *redirection_parse(char *cmdln);
int pending_input();
// The way C copy structs is, e.g. if struct wd c_wd, p_wd; p_wd = c_wd;
// p_wd and c_wd share the same memory of path which is manually allocated and freed.
// Therefore, we do not need another struct and pass struct from and to pwd() function,
// instead, we use a struct pointer *p_wd and pass struct pointer from and to pwd() function.
struct wd *pwd();
void cd(unsigned int idx);
void free_mem();
void execute();
void int_handler();
void piping();
void preexit();
void unpiping();

#endif
