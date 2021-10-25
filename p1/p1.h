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
struct ll_jobs *ll_bjobs;  // It needs to be freed, so is defined globally.
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

// The number of background jobs is not defined in p1_specifications, and is also not specified by Bash:
// https://unix.stackexchange.com/questions/272545/is-there-a-limit-to-processes-i-can-run-in-the-background
// so we have to use a linked list to store background jobs.
struct ll_jobs {
    bool stat;
    char* p_cmdln;
    pid_t pid;
    size_t jid;  // Job ID
    struct ll_jobs* next;
};

bool strcrpbrk_check(const char *s1, const char *s2, const char *s3);
bool parse(char *cmdln);
bool quote_check(char *ln);
char *strcrpbrk(const char *s1, const char *s2);
char *strcpbrk(const char *s1, const char *s2);
char *strqtok(char *parsedln, const bool p_quote[], size_t *p_tok);
char *redirection_parse(char *cmdln);
int pending_input();
struct ll_jobs *ll_jobs_getlast(struct ll_jobs *l_jobs);
struct ll_jobs *ll_jobs_init(char *p_cmdln, size_t n_job);
// The way C copy structs is, e.g. if struct wd c_wd, p_wd; p_wd = c_wd;
// p_wd and c_wd share the same memory of path which is manually allocated and freed.
// Therefore, we do not need another struct and pass struct from and to pwd() function,
// instead, we use a struct pointer *p_wd and pass struct pointer from and to pwd() function.
struct wd *pwd();
void cd(unsigned int idx);
void execute(bool f_bjob);
void free_mem();
void int_handler();
void jobs();
void ll_jobs_delete(struct ll_jobs *job);
void ll_jobs_insert(struct ll_jobs *l_jobs, struct ll_jobs *job);
void piping();
void preexit();
void unpiping();

#endif
