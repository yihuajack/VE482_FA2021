#include "p1.h"

void free_mem() {
    unsigned int i;
    for (i = 0; i < n_word; ++i)
        if (cmds[i])  // If not checking NULL, it seems that memory check gives no errors.
            free(cmds[i]);
    free(cmds);
    for (i = 0; i < n_pipe; ++i)
        if (pipe_fds[i])  // For multiple pipes we need to check every pipe_fd.
            free(pipe_fds[i]);
    free(pipe_fds);
}

void preexit() {
    free_mem();
    if (c_wd.path)
        free(c_wd.path);
    if (p_path)
        free(p_path);
}

char *redirection_parse(char *cmdln) {
    size_t i, j = 0;
    bool f_insquote = 0, f_indquote = 0;
    char *parsedln = (char *) malloc(sizeof(char) * CMD_MAX_LEN * 2);
    memset(parsedln, 0, CMD_MAX_LEN * 2);  // Mandatory, otherwise "No such file or directory"
    // This is important, otherwise if `ls` two times,
    // it will prompt "ls: cannot access ''$'\360\326''UUUU': No such file or directory".
    cmds = (char **) malloc(sizeof(char *) * CMD_MAX_LEN);
    memset(cmds, 0, CMD_MAX_LEN);
    // In this way we do not deal with the last character because it is always '\n'.
    for (i = 1; i < strlen(cmdln); ++i) {
        if (cmdln[i - 1] == '"' && !f_insquote)
            f_indquote = !f_indquote;
        else if (cmdln[i - 1] == '\'' && !f_indquote)
            f_insquote = !f_insquote;
        /* quote_stat[i] = ((f_indquote && cmdln[i] != '"') || (f_insquote && cmdln[i] != '\''));
         * if (cmdln[i - 1] == ' ')
         *   sp_in_quote[k++] = (f_indquote || f_insquote);  */
        parsedln[j++] = cmdln[i - 1];
        // Backward parsing
        if (!f_insquote && !f_indquote && (cmdln[i] == '<' || cmdln[i] == '>' || cmdln[i] == '|')
            && cmdln[i - 1] != ' ' && cmdln[i - 1] != '\t' && !(cmdln[i - 1] == '>' && cmdln[i] == '>'))
            parsedln[j++] = ' ';
        // Forward parsing
        if (!f_insquote && !f_indquote && (cmdln[i - 1] == '<' || cmdln[i - 1] == '>' || cmdln[i - 1] == '|')
            && cmdln[i] != ' ' && cmdln[i] != '\t' && !(cmdln[i - 1] == '>' && cmdln[i] == '>'))
            parsedln[j++] = ' ';
    }
    parsedln[j] = '\n'; // or cmdln[i], it is essential to quote parsing the end of line.
    free(cmdln);
    return parsedln;
}

/* strqtok() is different from strtok(), parsedln will not be changed,
 * thus we do not need to strtok(NULL) for the second time and we do not need to make a copy of parsedln.
 * We can make strqtok to behave similar to strtok() but that's unnecessary.  */
char *strqtok(char *parsedln, const bool quoted[], size_t *p_tok) {
    /* token = strtok(parsed_ln, TOK_DELIM);  original method
     * char *first_tok = strchr(parsedln + *tok_pos, ' ');  previous method
     * new_p_delim = (size_t) (strpbrk(parsedln + new_p_delim, " \t") - parsedln);  previous method  */
    /* "The newline character (\n), tab character (\t) and spaces will be considered as the same in the output."
     * Newline character (\n) is important for checking last command/end of parsing.
     * We have to take tab character (\t) the same as space.
     * First, find the first delimiter outside the "real" quotes.  */
    size_t i, j = 0;
    size_t delim_offset = strspn(parsedln, TOK_DELIM);  // Discard initial token delimiters
    parsedln += delim_offset;
    *p_tok += delim_offset;
    delim_offset = strcspn(parsedln, TOK_DELIM);  // Scan until the next token delimiter
    size_t p_newtok = *p_tok + delim_offset;
    while (delim_offset && quoted[p_newtok]) {
        delim_offset = strcspn(parsedln + delim_offset + 1, TOK_DELIM);
        p_newtok += delim_offset + 1;
    }
    // Next, exclude all "real" quotes from parsedln.
    char *token = (char *) malloc(sizeof(char) * (p_newtok - *p_tok + 1));
    for (i = 1; i <= p_newtok - *p_tok; ++i) {
        if (quoted[*p_tok + i] == quoted[*p_tok + i - 1]) {
            token[j++] = parsedln[i - 1];
        }
    }
    token[j] = '\0';
    *p_tok = p_newtok + 1;
    /* if cmdln contains only TOK_DELIM, token == '' ('\0'), (token) is still true, and '' will be a cmdbody:
     * ": command not found", and we cannot set token = NULL before it is freed, so we should process in main().  */
    return token;
}

bool quote_check(char *ln) {
    bool f_insquote = 0, f_indquote = 0;
    for (size_t i = 1; i < strlen(ln); ++i) {
        if (ln[i - 1] == '"' && !f_insquote)
            f_indquote = !f_indquote;
        else if (ln[i - 1] == '\'' && !f_indquote)
            f_insquote = !f_insquote;
    }
    return (f_indquote || f_insquote);
}

bool parse(char *cmdln) {
    bool f_firstio = 1;  // Flag of first cmdbody, 1 for empty
    bool f_insquote = 0, f_indquote = 0;
    int io_errno = 0;  // error number of I/O from 0 to 6
    f_i = 0;
    f_osingle = 0;
    f_odouble = 0;
    n_word = 0;
    n_pipe = 0;
    char *parsed_ln = redirection_parse(cmdln);
    // Quotes Parsing
    /* We can use some control character like DC4 ^T to replace spaces in parsedln, and check it in parse(),
     * but it is dirty. Instead, we use an array to record the status of any characters in parseln (char-by-char).
     * It is impossible to deal with quotes after strtok() without replacing (consider "  ").  */
    /* size_t quote_marks[CMD_MAX_LEN][2];  size_t *p_quote = (size_t *) malloc(sizeof(size_t));
     * Previous method records the beginning and the ending position of "real" quotes.
     * Previous method needs a size_t *p_quote to record which quoted region to scan, but that will not work.  */
    bool quoted[CMD_MAX_LEN] = {0};
    size_t i;
    /* The last character of parsed_ln '\n' should not be thrown away.
     * quote_marks[i][0] at 1 character after open quote and quote_marks[i][1] at 1 character before close quote.  */
    for (i = 1; i < strlen(parsed_ln); ++i) {
        if (parsed_ln[i - 1] == '"' && !f_insquote)  // Make sure f_indquote is reset to 0 in the beginning.
            f_indquote = !f_indquote;
        else if (parsed_ln[i - 1] == '\'' && !f_indquote)  // Make sure f_insquote is reset to 0 in the beginning.
            f_insquote = !f_insquote;
        // quoted[i] = (f_indquote && parsed_ln[i] != '"') || (f_insquote && parsed_ln[i] != '\'');
        quoted[i] = (f_indquote || f_insquote);  // The ending quote is counted as quoted for convenience
    }
    size_t *p_token = (size_t *) malloc(sizeof(size_t));
    /* gcc and clang's address sanitizer will change the behavior of programs.
     * Without address sanitizer, the two size_t pointers will be automatically initialized to 0 after malloc(),
     * but with address sanitizer, the initial value of the two pointers are some random large numbers.
     * Therefore, if we want to "exit", with address sanitizer, token will be always NULL,
     * and the program output is missing program error, since f_firstio is not set to 0.  */
    *p_token = 0;
    char *token = strqtok(parsed_ln, quoted, p_token);
    // To reset ifn and ofn is important, otherwise maybe unexpected behavior.
    memset(ifn, 0, FN_MAX_LEN);
    memset(ofn, 0, FN_MAX_LEN);
    /* if (*token == '\0') free(token);  if-else does not need token = NULL, no need to set n_cmd = 0.
     * The reason why we do not handle TOK_DELIM only cmdln here is we cannot differentiate " " and " abc"  */
    while (token) {
        if (io_errno)
            break;
        /* The additional check is used to deal with quoted ">". "<", "|", ">>".
        * Remember the ending quote is seen as quoted,
        * think about why we can handle both > and ">" by only 1 additional condition by this.  */
        if (token[0] == '>' && !quoted[*p_token - 2]) {
            if (f_osingle || f_odouble)
                io_errno = 2;
            if (strlen(token) == 2 && token[1] == '>')
                f_odouble = 1;
            else
                f_osingle = 1;
            free(token);
            token = strqtok(parsed_ln + *p_token, quoted, p_token);
            switch (token[0]) {
                case '>':
                    io_errno = 3;
                    break;case '<':io_errno = 4;
                    break;case '|':io_errno = 5;
                    break;default:strcpy(ofn, token);
                    break;
            }
        } else if (token[0] == '<' && !quoted[*p_token - 2]) {
            if (f_i || n_pipe)  // Deal with case "abc | grep abc < 1.txt" duplicated input redirection
                io_errno = 1;
            f_i = 1;
            free(token);
            token = strqtok(parsed_ln + *p_token, quoted, p_token);
            switch (token[0]) {
                case '>':
                    io_errno = 3;
                    break;
                case '<':
                    io_errno = 4;
                    break;
                case '|':
                    io_errno = 5;
                    break;
                default:
                    strcpy(ifn, token);
                    break;
            }
        } else if (token[0] == '|' && !quoted[*p_token - 2]) {
            if (f_osingle || f_odouble)
                io_errno = 2;
            /* https://man7.org/linux/man-pages/man3/exec.3.html
            * The list of arguments must be terminated by a null pointer, and,
            * since these are variadic functions, this pointer must be cast (char *) NULL.  */
            cmds[n_word++] = (char *) NULL;
            cmdbodies[++n_pipe] = n_word;
            // 7. Missing program. We have to give different results from Bash/Zsh. This is weird.
            // Originally, we use
            /*if (n_word && !cmds[n_word - 1])
            *     io_errno = 6;
            * else  */
            // at the beginning of this block, and we do not need the following 2 sentences,
            // then it can give the same result as Bash/Zsh.
            // However, to satisfy Task 12.7, we have to do like this.
            if (!cmds[cmdbodies[n_pipe - 1]])
                io_errno = 6;
        } else {
            f_firstio = 0;  // Must put here, otherwise m1c18 would fail.
            cmds[n_word] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
            memset(cmds[n_word], '\0', strlen(token) + 1);
            strcpy(cmds[n_word++], token);
        }
        free(token);
        token = (*p_token >= strlen(parsed_ln)) ? NULL : strqtok(parsed_ln + *p_token, quoted, p_token);
    }
    n_cmd = n_pipe + 1;// To derive cmdargs, the last command words should be counted.
    cmdbodies[n_cmd] = n_word;
    free(p_token);
    // strtok NULL will still retrieve parsedln, so we must free parsedln in the end.
    free(parsed_ln);
    // I/O error handler
    /* "At most one error will occur in a command, so you do not need to consider about the order of errors."
     * In this method, "missing program" has a lower priority,
     * because if there are pipes, io_errno will not be set here.
     * If first command does not have cmdbody, that's missing program.
     * If later commands do not have cmdbody, that's syntax error.  */
    if (!n_pipe && f_firstio)
        io_errno = 6;
    if (io_errno) {
        switch (io_errno) {
            case 1: printf("error: duplicated input redirection\n");
                break;
            case 2: printf("error: duplicated output redirection\n");
                break;
            case 3: printf("syntax error near unexpected token `>'\n");
                break;
            case 4: printf("syntax error near unexpected token `<'\n");
                break;
            case 5: printf("syntax error near unexpected token `|'\n");
                break;
            case 6: printf("error: missing program\n");
                break;
            default: break;  // Unreachable code
        }
        fflush(stdout);
        /* Here not piping yet, so we should not use free_mem() function,
         * otherwise, if execute "echo abc > /dev/permission_denied" and "echo abc < 1.txt < 2.txt"
         * ERROR: AddressSanitizer: attempting double-free  */
        for (i = 0; i < n_word; ++i)
            if (cmds[i])
                free(cmds[i]);
        free(cmds);
    }
    return !io_errno;
}

void piping() {
    pipe_fds = (int **) malloc(sizeof(int *) * (n_pipe + 1));
    for (unsigned int i = 0; i < n_pipe; ++i) {
        pipe_fds[i] = (int *) malloc(sizeof(int) * 2);
        if (pipe(&pipe_fds[i][0]) < 0) {
            fprintf(stderr, "error: pipe failed\n");
            preexit();
            exit(0);
        }
    }
}

void unpiping() {
    for (unsigned int i = 0; i < n_pipe; ++i) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }
}

int pending_input() {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    select(STDIN_FILENO + 1, &rfds, NULL, NULL, &timeout);
    return FD_ISSET(STDIN_FILENO, &rfds);
}

struct wd *pwd() {
    int path_conf = (int) pathconf(".", _PC_NAME_MAX);
    c_wd.path_len = (path_conf < 0) ? 255 : (unsigned int)path_conf;
    // When the first argument char *buf == NULL,
    // getcwd() will automatically malloc size_t size (the second argument),
    c_wd.path = (char *) realloc(c_wd.path, sizeof(char) * c_wd.path_len);
    if (getcwd(c_wd.path, c_wd.path_len)) {
        return &c_wd;
    } else {
        // fprintf(stderr, "error: getcwd failed with error %s.\n", strerror(errno));
        perror("error: getcwd failed.\n");
        preexit();
        exit(0);
    }
}

void cd(unsigned int idx) {
    unsigned int arglen = cmdbodies[idx + 1] - cmdbodies[idx];
    char *home = getenv("HOME");
    char *path = cmds[cmdbodies[idx] + 1];
    if (path[strlen(path) - 1] == '/')
        path[strlen(path) - 1] = '\0';
    if (!home)
        home = getpwuid(getuid())->pw_dir;
    if (arglen == 1 || (arglen == 2 && !strcmp(path, "~"))) {
        p_wd = pwd();
        chdir(home);
    } else if (!strcmp(path, "-")) {
        if (p_wd && p_wd->path) {
            // If you like, you can construct a struct for p_path,
            // then, you are actually doing a deep copy of p_wd/c_wd.
            /* Without checking the return value of realloc(), cppcheck: error:
            Common realloc mistake: 'p_path' nulled but not freed upon failure [memleakOnRealloc]
            https://archive.mantidproject.org/How_to_fix_CppCheck_Errors.html  */
            char *temp = (char *) realloc(p_path, sizeof(char) * p_wd->path_len);
            if (!temp)
                fprintf(stderr, "error: realloc failed.\n");
            else
                p_path = temp;
            strcpy(p_path, p_wd->path);
            p_wd = pwd();
            chdir(p_path);
            printf("%s\n", p_path);
        } else {
            p_wd = pwd();
            chdir(home);
            // fprintf(stderr, "prev_path is NULL\n");
            printf("%s\n", home);
        }
        fflush(stdout);
    } else {
        p_wd = pwd();
        if (chdir(path) < 0) {
            printf("%s: No such file or directory\n", path);
            fflush(stdout);
        }
    }
}

void execute() {
    unsigned int i, background = 0;
    /* Note: do not dynamically allocate memory for pid_list like this:
    pid_t *pid_list = (pid_t *)malloc(sizeof(pid_t) * n_cmd);
    otherwise displaying "mumsh $ 123" that is related to waitpid(),
    or "[1]    7208 illegal hardware instruction  ./mumsh" may be triggered.  */
    pid_t pid_list[PIPE_MAX_NUM];
    for (i = 0; i < n_cmd; ++i) {
        if (!strcmp(cmds[cmdbodies[i]], "exit")) {
            printf("exit\n");
            fflush(stdout);
            preexit();
            exit(0);
        } else if (!strcmp(cmds[cmdbodies[i]], "cd")) {
            cd(i);
        } else {
            pid_t pid = fork();
            pid_list[i] = pid;
            if (pid < 0) {
                fprintf(stderr, "error: fork failed\n");
                preexit();
                exit(0);
            } else if (pid == 0) {
                sigaction(SIGINT, &newhandler, &info);  // signal(SIGINT, SIG_DFL)
                sig_stat = 2;
                int stdout_back = STDOUT_FILENO;
                if (n_pipe && i < n_pipe && dup2(pipe_fds[i][1], STDOUT_FILENO) <= 0) {
                    fprintf(stderr, "error: pipe 1 redirecting failed\n");
                    free_mem();
                    exit(0);
                }
                if (n_pipe && i > 0 && dup2(pipe_fds[i - 1][0], STDIN_FILENO) < 0) {
                    fprintf(stderr, "error: pipe 0 redirecting failed\n");
                    free_mem();
                    exit(0);
                }
                /* "Input redirection can only be found in the first command,
                and output redirection can only be found in the last command,
                and each of them can occur only once."
                Hence, we only need one flag for each rather than a list of positions,
                or to bundle parsing into I/O redirections,
                and there is no need to reset f_i, f_osingle and f_odouble
                to ensure input and output redirections executed only once.
                We have to do input redirection only when (i == 0),
                and do output redirection only when (i == n_cmd - 1).  */
                if (!i && f_i) {
                    int fd0 = open(ifn, O_RDONLY);
                    if (fd0 <= 0) {
                        if (errno == ENOENT) {
                            printf("%s: No such file or directory\n", ifn);
                            fflush(stdout);
                        } else {
                            fprintf(stderr, "error: ifile failed to open\n");
                        }
                        exit(0);
                    }
                    dup2(fd0, STDIN_FILENO);  // STDIN_FILENO = 0
                    close(fd0);
                }
                /* "Only one output redirection and one input redirection is allowed in a command"
                Thus, here not using "else if" but "if" instead,
                to make sure it will succeed when there are both input and output redirection.
                Conditions like (n_cmd == 1 || i > 0) are not enough for Task 6.4.  */
                if (i == n_pipe && (f_osingle || f_odouble)) {
                    stdout_back = dup(STDOUT_FILENO);
                    int fd1 = (f_osingle) ? open(ofn, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)
                                          : open(ofn, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
                    if (fd1 <= 0) {
                        // If trying to open() directories like /etc, errno = EACCES
                        if (errno == EPERM || errno == EROFS || errno == EACCES) {
                            printf("%s: Permission denied\n", ofn);
                            fflush(stdout);
                        } else {
                            perror("error: ofile failed to open\n");
                        }
                        exit(0);
                    }
                    dup2(fd1, STDOUT_FILENO);  // STDOUT_FILENO = 1
                    close(fd1);
                }
                unpiping();
                // Remember if not builtin pwd, do not execvp(pwd).
                if (!strcmp(cmds[cmdbodies[i]], "pwd")) {
                    printf("%s\n", pwd()->path);
                    fflush(stdout);
                } else if (execvp(cmds[cmdbodies[i]], &cmds[cmdbodies[i]]) < 0) {
                    /* https://stackoverflow.com/questions/14500000/undo-redirect-of-stdout-with-dup2
                    If not backing up and resetting stdout, command not found error may be redirected to files.
                    For example, "abc > 1.txt" "cat 1.txt" output "abc: command not found",
                    the output will not show in stdout but in 1.txt.
                    The command not found error will clear the contents of 1.txt (consistent with Bash/Zsh).  */
                    if (i == n_pipe && (f_osingle || f_odouble) && dup2(stdout_back, STDOUT_FILENO) < 0)
                        fprintf(stderr, "error: cannot retrieve original stdout.\n");
                    printf("%s: command not found\n", cmds[cmdbodies[i]]);
                    fflush(stdout);
                }
                close(stdout_back);
                // Make sure child process will always exit.
                exit(0);
            } else {
                // Since following instructions still belongs to parent process,
                // there is no need to put any instructions in else {} block.
            }
        }
    }
    unpiping();
    if (background) {
        waitpid(-1, NULL, WNOHANG);
    } else {
        // while (waitpid(-1, NULL, WUNTRACED) != -1) {}
        // This method cannot handle multiple piped commands,
        // so we have to store a list of pid.
        for (i = 0; i < n_cmd; ++i)
            waitpid(pid_list[i], NULL, WUNTRACED);
    }
    // "Task 7: If there is an unfinished command, nothing should happen."
    // Without this, program will exit if Ctrl+D when parent process waiting for child process.
    if (pending_input()) {
        int c = getc(stdin);
        if (c == EOF)
            clearerr(stdin);
        else
            ungetc(c, stdin);
    }
    free_mem();
}

// https://stackoverflow.com/questions/231912/what-is-the-difference-between-sigaction-and-signal
// If you really want to use `signal()`,
// you can use `signal(SIGINT, int_handler2)` at the beginning of while (1) loop,
// where void int_handler2(int *sig_type) is
/* void int_handler2(int sig_type) {
    if (sig_type == SIGINT && !strlen(cmdln)) {
        printf("\nmumsh $ ");
        fflush(stdout);
    }
}  */
// and use `signal(SIGINT, SIG_DFL)` at the beginning of the chile process.
// This is just theoretically correct but implementation should be revised.
void int_handler() {
    sigaction(SIGINT, &info, NULL);
    if (sig_stat == 0) {
        sig_stat = 1;
        printf("\n");
        fflush(stdout);
    } else if (sig_stat == 2) {
        exit(0);
    }
}

/* strspn returns the length of the INITIAL portion of s1 containing only characters that appear in s2.
 * strcspn returns the length of the INITIAL part of s1 not containing any characters that are part of s2.
 * strpbrk returns a pointer to the character, or a null pointer if no character from s2 occurs in s1.
 * http://git.musl-libc.org/cgit/musl/tree/src/string
 * https://clc-wiki.net/wiki/C_standard_library:string.h  */
char *strcpbrk(const char *s1, const char *s2) {
    while (*s1)
        if (!strchr(s2, *s1++))
            return (char *)--s1;
    return 0;
}
/* Another implementation of strcpbrk():
 * char *strcpbrk(const char *s, const char *b) {
 *     s += strspn(s, b);
 *     return *s ? (char *)s : 0;
 * }
 * Use `strcpbrk_teset.cpp` to test execution time, we can see strcpbrk1 has better performance:
 * strcpbrk1: 1700 1100 900 900 1000
 * strcpbrk2: 2000 1200 1000 1100 1100  */

int main() {
    newhandler.sa_handler = &int_handler;
    while (1) {
        sigaction(SIGINT, &newhandler, &info);  // signal(SIGINT, int_handler);
        sig_stat = 0;
        int f_quoted = 0;
        char *cmd_ln = (char *) malloc(sizeof(char) * CMD_MAX_LEN);
        memset(cmd_ln, 0, CMD_MAX_LEN);
        char *cmd_sln = (char *) malloc(sizeof(char) * CMD_MAX_LEN);
        memset(cmd_sln, 0, CMD_MAX_LEN);  // Mandatory, otherwise "command not found"
        printf("mumsh $ ");
        fflush(stdout);
        /* We use a while(1) loop rather than while(fgets) because every time fgets fails,
         * we need to handle errors, but if we handle them after the while loop,
         * the handler will also be executed if fgets succeeds.
         * if block will always continue or break, while else block will always exit.  */
        while (1) {  // loop for one command line
            if (fgets(cmd_sln, CMD_MAX_LEN, stdin)) {
                if (quote_check(cmd_sln))  // we must do quote check here even it is partially repeated
                    f_quoted = !f_quoted;
                strcat(cmd_ln, cmd_sln);  // No matter what happened, we always concatenate them together first.
                if (f_quoted) {  // middle quoted lines (may separate), should not replace '\n' with ' '
                    printf("> ");
                    fflush(stdout);
                    continue;
                } else {  // first or last line, no need to replace '\n' with ' '
                    /* Process empty lines or cmdln with TOK_DELIM only
                     * It should never be parsed because once it is parsed, it is missing program.
                     * Last line cannot be empty or TOK_DELIM only, so only first line.
                     * In this stage, cmd_ln has been completed, and we should always break.  */
                    free(cmd_sln);  // after parsing there will be interrupts, so we have to free in advance.
                    if (!strcpbrk(cmd_ln, TOK_DELIM) || sig_stat == 1) {
                        free(cmd_ln);
                    } else if (parse(cmd_ln)) {  // first line with commands or last line
                        piping();  // The first possible preexit() here
                        execute();  // may exit(0) here
                    } else { // If !parse(cmd_ln), there is io_error, break to handle it.
                        free(cmd_ln);
                    }
                    break;
                }
            } else {
                // If fgets() fails, free memory, check errors, and exit.
                free(cmd_sln);
                free(cmd_ln);
                /* stackoverflow.com/questions/45261521/is-it-possible-to-distinguish-the-error-returned-by-fgets
                * Disinguish by feof() and ferror():
                * Ctrl+D is not a signal, but EOT (End of Transmission), which sends an EOF (End of File) to stdin.  */
                if (feof(stdin)) {
                    printf("exit\n");
                    fflush(stdout);
                } else if (ferror(stdin)) {
                    // Here must be else if, otherwise, Ctrl+C Ctrl+D will cause infinite loop of printf("exit\n")
                    if (errno == EINTR)
                        continue;  // continue "mumsh $ "
                    else
                        perror("error: fgets failed\n");
                }
                exit(0);
            }
        }
    }
}
