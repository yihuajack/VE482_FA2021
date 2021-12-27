# VE482_FA2021

### Honor Code Disclaimer

If there is same questions or labs in the future, it is the responsibility of JI students not to copy or modify these codes, or TeX files because it is against the Honor Code. The owner of this repository doesn't take any commitment for other's faults.

According to the student handbook (2020 version),

> It is a violation of the Honor Code for students to submit, as their own, work that is not the result of their own labor and thoughts. This applies, in particular, to ideas, expressions or work obtained from other students as well as from books, the internet, and other sources. The failure to properly credit ideas, expressions or work from others is considered plagiarism.

### Correctness

| Name                  | Score  | Out of | Mean | Comments                                                     |
| --------------------- | ------ | ------ | ---- | ------------------------------------------------------------ |
| h1                    | 46     | 50     | 39.3 | Ex.4 -2 0.244 KB<br />Ex.5 -2 no README file                 |
| h2                    | 75     | 75     | 66.4 |                                                              |
| h3                    | 45.9   | 60     | 41.5 | Ex.1 1. -1 threads cooperation<br />4. -8<br />-10% late **  |
| h4                    | 20     | 50     | 31.3 | ***                                                          |
| h5                    | 80     | 60     | 57.6 |                                                              |
| h6                    | 35     | 60     | 40.6 |                                                              |
| h7                    | 52     | 60     | 39.4 |                                                              |
| h8                    | 42     | 40     | 45   |                                                              |
| l1                    | 95.5   | 100    | 84.6 | CPU heat issue. -0.5<br />2>&1 > What about stderr? -1<br />Linux file structure. -3 |
| l2                    | 90     | 90     | 75.3 |                                                              |
| l3                    | 80     | 90     | 65.1 | Use rsync to copy. -5<br />How to find active IP? -5 *       |
| l4                    | 86     | 100    | 78.6 | 1.3.5 -9 None of the numbers are accurate. The first two are close (15880, 36956) so I did not deduct, but the last three are kind of off (63301,59933,63807).<br />2.1.6 -5 Did not provide what "reverse step" is. |
| l5                    | 100    | 100    | 73.9 |                                                              |
| l6                    | 90     | 90     | 38.9 |                                                              |
| l7                    | 90     | 90     | 71.6 |                                                              |
| l8                    | 94     | 100    | 84.2 | -6 2.2.1 In Minix, LRU is implemented in /servers/vm/region.c, where a doubly linked list of yielded_t is kept. |
| l9                    | 83.52  | 90     | 63   | -3.6% What about return 0? -3.6% cdev_add cannot create node under /dev |
| l10                   | 63     | 90     | 58.9 | -2 2.1.1 How to ensure the new version of PATH is then forgotten? -5 mum will recognize the fake su -10 gp-2.12 not able to successfully do the job -10 lacking gp.service file |
| l11                   | 70     | 100    | 45.2 | -30 code                                                     |
| p1 presentation slide | 17.6   | 20     | 17.4 |                                                              |
| p2 presentation slide | 25     | 30     | 21.8 |                                                              |
| p1.3                  | 127.27 | 100    | 99.8 | base score: 520 / 520 bonus score: 200 / 220 total score: 520 / 520 * 100 + 200 / 220 * 30 |
| p2.3                  | 7.5    | 100    | 46.1 |                                                              |
| p3.3                  | 126.5  | 100    | 57.8 |                                                              |

*: See `l3/l3.txt`. For bullet 2, TA's explanation:

> For section 2.1, the exercise "Create an exact copy of the directory /usr/src into the directory /usr/src_orig", you should use rsync since this is an exercise under rsync section.

**: It was submitted on Oct 21 at 12am (late)

***:

Ex.1
1.-5 2. -5 Ex.2 1. -5 Ex.4 -15 $ clang -lpthread -o ex4 ex4.c && ./ex4 ex4.c:14:18: warning: incompatible pointer types passing 'sem_t *\*' to parameter of type 'sem_t *'; remove & [-Wincompatible-pointer-types]        sem_wait(&sem);                 \^\~\~~ /usr/include/semaphore.h:55:29: note: passing argument to parameter '\_\_sem' here extern int sem_wait (sem_t *\_\_sem) \_\_nonnull ((1));                            ^ ex4.c:18:18: warning: incompatible pointer types passing 'sem_t **' to parameter of type 'sem_t *'; remove & [-Wincompatible-pointer-types]        sem_post(&sem);                 ^~~~ /usr/include/semaphore.h:78:29: note: passing argument to parameter '\_\_sem' here extern int sem_post (sem_t *_\_sem) _\_THROWNL __nonnull ((1));                            ^ 2 warnings generated. The futex facility returned an unexpected error code. The futex facility returned an unexpected error code. [1]    6304 abort (core dumped)  ./ex4

### Grade Weights

| Group     | Weight |
| --------- | ------ |
| Homework  | 10%    |
| Exams     | 40%    |
| Labs      | 10%    |
| Project 1 | 7.5%   |
| Project 2 | 20%    |
| Project 3 | 12.5%  |

*I put this table here because you will not be given the weight of each project before the final grades are released. As is presented, Project 1 + Project 3 = Project 2.*

### Announcements

#### [P1] Thing you need to know about project 1 (Sep 16)

> We will have three projects this semester, and you can already find project 1 on Canvas and JOJ. Here are some things you need to know. 
>
> \1. You are restricted to using the C programming language for project 1, with any C standard. The language on JOJ is fixed so you won't pass it if you use other languages. And the C standard on JOJ is fixed to GNU11. If you fail to pass JOJ just because you use another lower version of the C standard (e.g. C99), please contact us.
>
> \2. Due to the brevity of the P1 manual, we have prepared for you a compile guide and a set of specifications, both of which can be found in the Files section. 
>
> \3. All of you have a project 1 repository on [Gitea (Links to an external site.)](https://focs.ji.sjtu.edu.cn/git/ve482-21). Please use git to control the version of your source code and push your work to Gitea. The usage of git will be graded in the future.
>
> \4. To help all of you to start with the project, you and your classmates will be giving presentations in groups of 4 in the next two weeks. Form your own groups by joining a "p1-pre" group in the People section. We will randomly assign you to a group if you still have not joined one by Sep.18 23:59.
>
> In week 2 and week 3, we will have **only one** lab session per week:
>
> - **Time**: September 23nd and 30th (Thursday) 18:20-20:55
> - **Venue**: Room 211 @ East Lower Hall (东下院)
>
> According to the milestones and your progress on the project, we divide the topics into 2 sections. The first section will be presented in lab 2 and the second in lab 3.
>
> ### Lab 2 Topics - To get you started with `mumsh`
>
> 1. What are system calls and why we need system calls for shell
>
> 2. What are parent/child processes, and usage of `fork()` and `wait()`/`waitpid()`
>
> 3. How to execute a simple command, such as `ls`, both with/without argument (`execvp()`)
>
> 4. Bash style redirection syntax, e.g.
>
>    ```
>    <1.txt>3.txt cat 2.txt 4.txt
>    ```
>
> 5. Quotation marks in shell scripts, e.g.
>
>    ```
>    echo "de'f' ghi" '123"a"bc' a b c
>    "echo" "<
>    1.'txt'"
>    ```
>
> 6. Bash pipeline syntax, e.g.
>
>    ```
>    echo 123 | grep 1
>    ```
>
>    and implementing pipeline (`pipe()`)
>
> 7. Implementing redirection (`dup2()`/`dup()`)
>
> 8. Opening (`open()`) and closing (`close()`) files. What is the connection between file permission and `mode` in the parameter of `open()`?
>
> ### Lab 3 Topics - to help you organize your project better
>
> 1. How to organize the read/parse/execute loop (how to have a better structure organization of the shell code, e.g. for further extension/modification)
>
> 2. How to parse input (consider all cases including without arguments, with arguments and redirection)
>
> 3. Problems likely to lead to memory leak and how to prevent/solve
>
> 4. Implementation of built-in commands
>
> 5. Signals and signal handler (`signal()`, handling `CTRL`+`C`)
>
> 6. What are background processes and their output format, such as
>
>    ```
>    ls &
>    ```
>
> 7. Identification of possible errors, such as failing to fork and
>
>    ```
>    echo abc > | grep abc # syntax errors
>    echooo b # unable to find command
>    ```
>
>    and error handling (`perror()` or `printf()`).
>
> Each group can select one topic from the above 15 topics. Sign up for the topics in the "Calendar" section on Canvas. Remember to tick the box in front of VE482. Please discuss with your group members which topic to choose. We follow the rule of first-come, first-served. Each presentation should be roughly **4-10 minutes**.
>
> At least one of the group members have to show up during the presentation. For those who have trouble attending the lab sessions, try to help your group members to find information and prepare the slides. You can check the recordings afterward. For online students, you can also try to give the presentation over the online meeting.
>
> If you have any questions, feel free to contact us via Piazza or email.

#### [P1] code quality check (Sep 21)

> In order to ensure the code quality of p1, an extra flag `-Wconversion` is added for the compilation.
>
> Plus, we will use [cpplint (Links to an external site.)](https://github.com/cpplint/cpplint) and [cppcheck (Links to an external site.)](https://github.com/danmar/cppcheck) to check your code quality. The command we use will be `cpplint --linelength=120 --filter=-legal,-readability/casting,-whitespace,-runtime/printf,-runtime/threadsafe_fn,-readability/todo,-build/include_subdir,-build/header_guard *.h *.c` and `cppcheck *.h *.c`. You will be deducted if you can not pass all of them.

#### [Lab 2] Install Minix (and Linux, if you do not have it already) (Sep 22)

> If you do not have [Minix 3 (Links to an external site.)](https://www.minix3.org/) installed, please install it as soon as possible. You will use it a lot in VE482, starting from tomorrow's lab. You will be installing it on a virtual machine. For the Minix version we recommend Minix 3.2.1. For the virtual machine I personally recommend VMware Workstation. Installing a graphical user interface is not recommended. You will find tutorials on the Internet with much ease. If you encounter any problems, we can take a look at it together on tomorrow's lab.
>
> Please also have a copy of Linux at your disposal. No specific version is required. You can also install it on a virtual machine. Windows subsystem on Linux (WSL) only works for half of the semester because you will need to edit the Linux kernel later on in the semester. 

#### [Lab 2] Fixes to certain Minix 3.2.1 issues (Sep 23)

> We have found a solution to "Could not fetch [package name] " on Minix 3.2.1 when running "pkgin update". 
>
> The package source of Minix3.2.1, which begins with "ftp://", is already unavailable. But another, which begins with "http://", is still available. So we directly replace the original package with the http one. 
>
> In Minix, open the configuration file:
>
> ```
> vi /usr/pkg/etc/pkgin/repositories.conf
> ```
>
> Replace the line beginning with "[ftp://xxxx", (Links to an external site.)](ftp://xxxx%2C/) to
>
> ```
> http://www.minix3.org/pkgsrc/packages/$osrelease/$arch/All
> ```
>
> (Note: You will be doing this with vi, which is not the best text editor in the world. Unfortunately it is your only choice right now. Search a bit on the internet about how it works. You can install nano or vim afterwards.)
>
> It should be working after you change this line. 
>
> If it is not, it means your virtual machine itself is not connecting to the Internet properly. Remember to let your virtual machine use NAT. For VMware Fusion users, go to "Virtual Machine"-"Settings"-"Network Adapter"-Enable "Share with my Mac". For VMware Workstation users, go to VM-Settings-Network Adapter. Select NAT. 
>
> Another common problem is that after you reboot, what you edited or configured before is lost. This is because you are still booting from the iso after you have already installed Minix. You need to switch to booting from your hard drive. For VMware Fusion users, go to "Virtual Machine"-"Settings"-"CD/DVD (IDE)"-Disable booting from CD/DVD. Then go to "Startup Disk" and restart from the disk. For VMware Workstation users, go to VM-Settings-CD/DVD(SATA). Select "Use physical drive". 
>
> Also, if you changed your Ethernet card to "8" using "netconf", please change it back to your default option.
>
> We are sorry that you did not have a chance to follow along in the lab, and you can find some notes about ssh in the files section. You can also find the recording in the Feishu group. We are extending the due of Lab 2 report by one day due to the confusion caused by Minix installation. 

#### [P1] Task 10, 11, 13 become optional (Sep 25)

Task 10, 11, 13 become optional in project 1. The rest of the tasks will sum up to 100 in project 1 and the score you get in task 10, 11, 13 will be counted as bonus.

#### [Lab 1] About Submission in Markdown (Sep 30)

> I noticed that some of you submitted a single Markdown file as your Lab 1 report. You should notice that images in Markdown are all relevant references, rather than really included in the document. So, if you submit only a Markdown file, the images cannot show up. In future labs, please submit a PDF file (preferred) as the report or a zip file.
>
> As for those who submitted a single Markdown file, please send the PDF or zip file to [my email](mailto:citrate@sjtu.edu.cn) ASAP. Do not change the text content of your report and we will compare it with your originally submitted one. If you change the text content, it may be viewed as a violation of the Honor Code.

#### Minix, group pair and ssh keys (Sep 30)

> We have maintained a mirror of `pkgin` on our server. You can edit `/usr/pkg/etc/pkgin/repositories.conf`, and type in `http://focs.ji.sjtu.edu.cn/minix/$arch` to change the source. Notice, it only supplies packages for Minix 3.2.1, please make sure you have installed M3.2.1, but not 3.3.0.
>
> Now you can establish your group pair. It will be used to form the group of p2, p3, and many labs. So choose your partner wisely. If you do not finish grouping at October 5th 23:59, you will be arranged randomly.
>
> And make sure you have uploaded your key to gitea, otherwise you will not be able to access our 64 cores, 126 GB RAM server to test p2.

#### Server ready (Oct 4)

> Now you can access ve482 server via ssh (using the public key you have uploaded to Gitea). You will need to connect to SJTU VPN first to connect to it if you are off-campus.
>
> ```
> ssh <YOUR_STUDENT_ID>@111.186.59.59
> ```
>
> P2 related files will be ready when it is released.

#### [P1] One-week due extension (Oct 5)

> The due for p1 is pushed for one week, to Oct. 13, 23:59. You will get a **+5% bonus** if you finish it before the original deadline (i.e. the time of your last commit on Gitea is before Oct. 6, 23:59). 

#### [P1] more about P1 (Oct 5)

> You still need to catch up the milestone 2 at the original due of p1, i.e. Octo 6th 23:59, or you will get deducted.
>
> You need to provide a release on Gitea as the final submission of p1. The procedure is rather simple.
>
> 1. Make sure the latest commit of the master branch contains the work you need to submit. No nested file structure, we will compress it directly and send it to JOJ.
> 2. Visit [https://focs.ji.sjtu.edu.cn/git/ve482-21//releases/new (Links to an external site.)](https://focs.ji.sjtu.edu.cn/git/ve482-21//releases) .
> 3. Input "p1" in both "Tag name" and "Title", no need to upload anything.
> 4. Click "Publish Release".
>
> If you are not sure, check [Releases - student - FOCS git server (sjtu.edu.cn) (Links to an external site.)](https://focs.ji.sjtu.edu.cn/git/ve482-21/student/releases) as an example.
>
> And tips: Do not forget P1 code quality check. Form your group pair. Get ready for P2.

#### [P2] project 2 released (Oct 7)

> Project 2 is released.
>
> Some tips:
>
> - Read through the whole project manual carefully.
> - Connect to the company's server with your student ID as the username.
> - Find relative files in `/opt/lemondb/` on the server and your gitea repository.
> - Your code will be tested on the server for final grade, so make good use of it.
> - The master branch on your group repository on gitea is protected. You need to create a new branch for developing and raise a PR, waiting for approval by your group mates to merge it to the master branch.
> - You need to change your username in the git repository to make it consistent with your gitea full name. We need to count your contribution based on this.
>   - e.g.: in your git repo directory, run `git config user.name "张三518370910000"`
> - Start early, you will become busier soon.
> - `/opt/lemondb/bin/lemondb` on the server is fast, but not fast enough. Treat it as a baseline.
> - JOJ can be used for simple test, but you will definitely need a script to test larger and more complicated queries.

#### [labs 5, 6, 7 & p2] P2 presentations (Oct 12)

> To help you understand project 2 better, we will hold presentations in labs 5, 6, and 7. According to the milestones and your progress on the project, we have divided the topics into 3 sections, one for each lab. You will be giving presentations with your **group-pair** partner. Each presentation should be roughly **3-7 minutes**. For these three labs, we will have **only one** lab session per week:
>
> - **Time**: October 21st, 28th, and November 4th (Thursday) 18:20-20:55
> - **Venue**: Room 211 @ East Lower Hall (东下院)
>
> ### Lab 5 Topics - Tricks Related to Special C++ Functionalities
>
> 1. **(lambda expressions)** What are lambda expressions in C++ and what is the basic syntax? What are capture clauses?
> 2. **(rvalue reference and move)** What is lvalue, rvalue and rvalue reference in C++11? What is `std::move` function? Explain both the concepts and usages.
> 3. **(scopes)** What is the concept of scopes in C/C++? What is the storage duration of a variable? When will the constructor and destructor of an object be called? Consider the `std::move` function and explain why it can save time.
> 4. **(smart pointer)** What are smart pointers C++11? What are their advantages compared to the low level memory management (`new`/`delete`)? What is the difference between `unique_ptr` and `shared_ptr`? Explain how they can be powered by rvalue reference and move.
> 5. **(function)** What are function objects in C++11? What is the relationship between function pointers, lambda expressions and `std::function` objects?
> 6. **(iterator)** How to implement a STL style iteration interface?
> 7. **(exceptions)** How to use exception in C++? Why `noexcept` keyword is added in C++11? Why the exception in C++ is considered slow sometimes?
> 8. **(object oriented programming)** What is the usage of `explicit` and `=default`? Discuss all types of constructors and assignments in C++11. What is a member initializer list?
> 9. **(object oriented programming)** What are virtual functions and abstract classes? What is the `friend` keyword and why should it be used as little as possible? Why are the `override` and `final` keywords added in C++11?
> 10. **(basics)** How to use `getopt_long`? How to debug using `#define` and how to enable/disable debug mode during compilation?
> 11. **(data structure)** What data structure does LemonDB use to organize its tables? What are B-trees and B+ trees? Why do modern database management systems such as SQLite use them? 
>
> ### Lab 6 Topics - C++ Functionalities (Contd.), Database Essentials and Git
>
> 1. **(auto and decltype)** What are the `auto` and `decltype` keywords? How can they be applied in `for`-range loops and lambda expressions in C++11?
> 2. **(emplace and forward)** When should we use `emplace` instead of `push` in STL containers? What is parameter pack in C++11? How to use `std::forward` to apply this trick?
> 3. **(iterator)** What is the structure of `std::iterator` and when is it used? Provide code examples to explain how to use it in a custom class.
> 4. **(STL)** How to use basic data structures of STL like `std::unordered_map`, `std::deque`, and how are they implemented? Why are we using `std::unordered_map` for databases (think in terms of time complexity)? Are they thread-safe?
> 5. **(basics)** What are inline functions, `constexpr` specifiers, and the `static` keyword in C++? When and how should we use them?
> 6. **(database structure)** Explain the concept of row- and column-oriented database management system (DBMS). Which is more efficient? What kind of database is LemonDB?
> 7. **(git)** How do you develop a project with 4 group members together? If there are 10 developers, what will happen? And if there are 100, 1000? Will your methods still be available in this condition? Discuss how to improve it to satisfy unlimited contributors. You can refer to how huge projects on GitHub works.
>
> ### Lab 7 Topics - Multithreaded Programming and Database Management 
>
> 1. **(multi-threading)** Explain the general database concept and common issues related to multi-threading in this context.
> 2. **(multi-threading)** What are pop-up threads and why do they fit well in this project?
> 3. **(multi-threading)** How to split a query into subqueries to fully use all the cores?
> 4. **(mutex and semaphore)** How to use `mutex` and `semaphore` in C++? If several threads call mutex-lock when a mutex is already locked, when this mutex is unlocked, which thread will acquire this mutex?
> 5. **(database queries)** Discussion/summary/clarification about the `LISTEN` instruction.
> 6. **(database queries)** How does the parser transform query into internal commands?
>
>  Each pair can select one topic from the above 24 topics. Sign up for the topics in the "Scheduler" Tab under the "Calendar" section on Canvas. Remember to tick the box in front of VE482. Please discuss with your partner which topic to choose. We follow the rule of first-come, first-served. If you do not choose at all, you will receive a zero for this presentation. 
>
> At least one of the pair members has to show up during the presentation. For those who have trouble attending the lab sessions, try to help your partner to find information and prepare the slides. You can check the recordings afterward. For online students, you can also try to give the presentation over the online meeting.
>
> If you have any questions, feel free to contact us via Piazza or email.

#### [P1] Pre-check done (Oct 12)

> P1 will be due in Octo. 13th at 23:59. No late submission will be accepted after that moment.
>
> Pre-check of p1 is done half an hour ago. The feedback comes as an issue in your Gitea repo named after "p1 submission pre-check failed". If you do not receive that, which means your p1 repo on Gitea is up to the following standard:
>
> 1. a release named after p1
> 2. at least one .c file in the root directory
> 3. a file which name starts with "readme" (case insensitive) in the root directory
>
> The pre-check will be run again tomorrow at lunchtime. Check [Here](https://umjicanvas.com/courses/2257/discussion_topics/38472) for submission details.

#### [P1] About early submission with no README file (Oct 13)

> If you release your p1 before the original due (means that you will have bonus for that) but without a README file, you can just commit a README file to master branch without release it. We will check that README file. And do not forget to release at least one version before due or you will get 0 for p1. Our recommendation is to release at least one version now.

#### [L6] Hints (Oct 29)

> Since zathura is not well documented, we find some useful links for you:
>
> - [https://git.pwmt.org/pwmt/zathura/-/blob/develop/doc/api/plugin-development.rst (Links to an external site.)](https://git.pwmt.org/pwmt/zathura/-/blob/develop/doc/api/plugin-development.rst)
> - [https://git.pwmt.org/pwmt/zathura/-/blob/develop/zathura/plugin-api.h#L107 (Links to an external site.)](https://git.pwmt.org/pwmt/zathura/-/blob/develop/zathura/plugin-api.h#L107)
>
> You may also refer to other plugins to develop your own plugin. But keep in mind that though in the README of the plugins they say they should be built with `make install,` actually, they need to be built with `meson` just like zathura itself. Show how to build your plugin correctly in your README file.
>
> And the example code for rendering a "hello world" on 40, 40 of the page by implementing `page_render_cairo` is here:
>
> ```
> zathura_error_t
> txt_page_render_cairo(zathura_page_t* page, void* data,
>     cairo_t* cairo, bool UNUSED(printing))
> {
>   if (page == NULL || cairo == NULL) {
>     return ZATHURA_ERROR_INVALID_ARGUMENTS;
>   }
>   
>   zathura_document_t* document = zathura_page_get_document(page);
>   if (document == NULL) {
>     return ZATHURA_ERROR_UNKNOWN;
>   }
>  
>   cairo_set_source_rgb(cairo, 0.1, 0.1, 0.1);
>  
>   cairo_select_font_face(cairo,"Times New Roman",
>       CAIRO_FONT_SLANT_NORMAL,
>       CAIRO_FONT_WEIGHT_BOLD);
>  
>   cairo_set_font_size(cairo, 13);
>  
>   cairo_move_to(cairo, 40, 40);
>   cairo_show_text(cairo, "hello world"); 
>  
>   return ZATHURA_ERROR_OK;
> }
> ```
>
> If you implement the other functions correctly, it will work fine.

#### [L3] Some Advice (Oct 30)

> The grade of Lab 3 is released. Here's some advice:
>
> 1. Attach your command/code in a separate file. Or at least use a listing package in your LaTeX document. The worst thing to do is to paste your command into LaTeX directly. Since there are escape characters in LaTeX, your command can be totally wrong. In future labs, if your command becomes useless because of such copy-and-paste behavior, we will no longer grade it.
> 2. In the rsync part, you should always use rsync to do a copy, rather than cp.
> 3. ifconfig default shows "UP" IPs. However, you should state this point. You can also use "ifconfig -a" and try to find those IPs with a "UP" flag. Another trick is to directly pass the name of an active hardware to ifconfig. If you didn't do anything and retrieve IPs from ifconfig directly, 5 points are deducted.
> 4. We find many of you have exactly the same command for some questions. Please always pay attention to the Honor Code and if your work always shows similarities with other's, you may be accused to the Honor Council.

#### [L6] Some Clarifications (Nov 1)

> Many of you asked me about whether or not the third question of Lab 6 is a bonus. Here I want to clarify that:
>
> - If you successfully write a **compilable** skeleton of the Zathura plugin, 15 points are earned.
> - If your Zathura plugin is fully functional, 5 additional points are earned.
> - If you successfully write a **compilable** skeleton of the csv plugin, 15 points are earned.
> - If your csv plugin is fully functional, 5 additional points are earned.
>
> All 40 points above are part of the 90 total points, which means none of them are "bonuses". However, you can see a fully functional plugin has a less point-weight, meaning that you can treat them as bonus and decide not to finish them.
>
> If you don't want to implement a fully functional plugin, to make it clear that your plugin is compilable, you'd better give it an indicator. For example, in the Zathura plugin, you can output a page with "Hello World" to indicate your plugin is actually called. Similarly, you can output "Hello World" when your csv plugin is called.
>
> About how to use cairo to render a page, please refer to the [previous announcement](https://umjicanvas.com/courses/2257/discussion_topics/39148).

#### [P2] Bonus for Pre-commit and Conventional Commits (Nov 2)

> As mentioned during the lecture, we will give a small bonus on p2 to groups who use [pre-commit (Links to an external site.)](https://pre-commit.com/) and follow [conventional commits (Links to an external site.)](https://www.conventionalcommits.org/en/v1.0.0/). They help you to make your commits more normative and reliable.
>
> Recommended hooks:
>
> - [https://github.com/pre-commit/pre-commit-hooks (Links to an external site.)](https://github.com/pre-commit/pre-commit-hooks)
> - [https://github.com/pre-commit/mirrors-clang-format (Links to an external site.)](https://github.com/pre-commit/mirrors-clang-format)
> - [https://github.com/compilerla/conventional-pre-commit (Links to an external site.)](https://github.com/compilerla/conventional-pre-commit)
> - [https://github.com/pocc/pre-commit-hooks (Links to an external site.)](https://github.com/pocc/pre-commit-hooks)

#### [P2]\[Mandatory] Drone is online for CI (Nov 2)

> Hope you have all checked with pre-commit and conventional commits for a more professional and reliable git workflow as mentioned in the previous announcement. Pre-commit can do the check for you automatically before a commit to prevent error / wrong format / etc. in your commit. And conventional commit messages can make your commit message more readable and easy to trace the source of an error.
>
>  
>
> Now we introduce you to our newly deployed Continuous Integration platform: [Drone (Links to an external site.)](https://focs.ji.sjtu.edu.cn/drone). You can check its document here: [Drone CI / CD | Drone (Links to an external site.)](https://docs.drone.io/). And if you are not familiar with CI, check out here: [Continuous integration - Wikipedia (Links to an external site.)](https://en.wikipedia.org/wiki/Continuous_integration). CI test empowers you to automate the build, test workflows.
>
> In ve482, we ask you to use CI to test your code in p2. And branch protection is enabled for the master branch. Your code needs to pass the CI test, which should at least compile successfully, before merging to master.
>
> For an example in ve482, check [ve482-21/student - student - FOCS git server (sjtu.edu.cn) (Links to an external site.)](https://focs.ji.sjtu.edu.cn/git/ve482-21/student) and [ve482-21/student - Drone CI (sjtu.edu.cn) (Links to an external site.)](https://focs.ji.sjtu.edu.cn:2222/ve482-21/student). As you can see, a new file /`.drone.yml` is created in the develop branch. Check [Overview | Drone (Links to an external site.)](https://docs.drone.io/pipeline/ssh/overview/) to learn the meaning of each line in that file. Note that for any non-zero return value in the commands, the whole pipeline will stop and give an error. You should configure your own workflow to at least build your lemondb successfully on the server. The whole group will get deducted if you do not set up CI test correctly.
>
> Tips: The variable name in from_secret is set here: [Settings - ve482-21/student - Drone CI (sjtu.edu.cn) (Links to an external site.)](https://focs.ji.sjtu.edu.cn:2222/ve482-21/student/settings/secrets). For me, host is set to 111.186.59.59, username is set to frown, and ssh_key is my private key. So that the commands will be executed on the server via ssh. DO NOT forget to check "Allow Pull Requests" when creating a new secret. Or your secrets will be unreachable when the pipeline is triggered by a PR.


#### [H5 & Labs] Less work and more workers (Nov 3)

>We have decided to make hw5 ex4 optional, such that you are able to get full points even without finishing it. The marks originally dedicated to this exercise will serve as a bonus for anyone who finishes it.
>
>Starting from lab 7 (this week's lab), labs will become groupwork and you will work on labs with your project-pair partner. You only need to make one submission per pair. 

#### [URGENT,IMPORTANT] Code quality (Nov 4)

> As we all know, code quality plays a very important role in software development. We aim to write clean, well-documented, and maintainable code for helping further developments. Codes with low quality make everyone suffer.
>
> But many of you have missed the P1 code quality announcement. For those who can not pass the code quality check, an extra 1% deduction is applied to your "p1 deduction" canvas grade. However, if you can not pass the code quality check before Nov 5th 23:59, a large deduction will be applied.
>
> JOJ for p1 is reopened. And you need to fix the issues in your code and commit, push, release to Gitea with the name "p1-fix" before Nov 5th 23:59. You should keep your code functionalities unchanged. So the lower grade of your previous "p1" release and the "p1-fix" release will be your final p1 grade.
>
> Commands used to check your code quality (looser than the previous announcement) are listed below. And they will output nothing if the code quality of your P1 is good enough.
>
> ```bash
> $ find . -name '*.c' -or -name '*.h' | xargs cppcheck --quiet --language=c
> $ cpplint --linelength=120 --filter=-legal,-readability/casting,-whitespace,-runtime/printf,-runtime/threadsafe_fn,-readability/todo,-build/include_subdir,-build/header_guard,-readability/braces,-build/include_order,-runtime/int,-build/include_what_you_use --quiet --recursive .
> ```
>
> Now we also add code quality check to P2. Since P2 is a larger project, simply using `cpplint` is not enough. We use `clang-tidy` as a replacement for it. And the full new version of the recovered files can be accessed here: [ve482-21/lemondb - lemondb - FOCS git server (sjtu.edu.cn) (Links to an external site.)](https://focs.ji.sjtu.edu.cn/git/ve482-21/lemondb). You can check the differences via git commit history. A small bonus will be given to those groups who use it. And please add clarification in the README file if you use it.


#### [P2] Submission & Gitea usage (Nov 11)

>For p2 submission, you need to create a release named after p2 in your repository. And do not forget about the wiki.
>
>We will build your program with release mode in CMake. Make sure your program can run in release mode!
>
>```bash
>$ mkdir build && cd build
>$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ../src
>$ cmake --build .
>$ cd ..
>$ ./build/lemondb --listen <QUERY_FILE_NAME>
>```
>
>As mentioned during lab and the lecture on Tuesday, make sure you use Gitea (and also git) properly.
>
>- add drone badge to your README file like this: ![img](https://focs.ji.sjtu.edu.cn:2222/api/badges/ve482-21/student/status.svg)
>- open an issue for each task (and document what is to be done for it)
>- create a project with at least 3 extra boards (in progress, in review, completed)
>- create m2 and m3 milestones
>- for each issue set:
>  - the branch where it is developed
>  - a label
>  - the milestone it belongs to
>  - the project
>  - assign someone to work on it
>
>when you start working on an issue, in the project move it on "in progress", then to "in review" when you issue a PR, and finally "completed" when the PR is fully approved.

#### [P3 & Labs] Something ends, something begins (Nov 17)

> Project 2 will be due soon and project 3 will be released next week. **Please fill in the [Lemonion Happy Workplace Initiative](https://umjicanvas.com/courses/2257/quizzes/4181) as early as possible**, and we may adjust the project 3 groups according to the survey results. 
>
> To help you get started with project 3, we will hold presentations in labs 9 and 10. You will be giving presentations with your new **project 3 group (3-4 people)**. Each presentation should be roughly **5****-7 minutes**. 
>
> For all the remaining labs, we will have **only one** lab session:
>
> - **Time**: Thursday 18:20-20:55
> - **Venue**: Room 211 @ East Lower Hall (东下院)
>
> ### Lab 9 Topics - Basic Scheduling Concepts
>
> 1. (**MINIX**) What is the MINIX process table? What is its structure?
> 2. (**MINIX**) What are messages and how can they be used for user-kernel space communications?
> 3. (**Scheduling**) What is Lottery Scheduling? How is it usually implemented?
> 4. (**Scheduling**) What is EDF Scheduling? How is it usually implemented?
> 5. (**Scheduling**) How to announce a deadline during EDF scheduling?
> 6. (**Scheduling**) What is the `nice` command? How is it implemented in MINIX3?
>
> ### Lab 10 Topics - Modifying Kernel Code
>
> 1. (**MINIX**) How to disable user space scheduling and switch to kernel space scheduling?
> 2. (**MINIX**) Where does MINIX 3.2.1 handle scheduling? What is the default scheduling algorithm?
> 3. (**MINIX**) What functions need to be modified for this project?
> 4. (**MINIX kernel**) How to write a new kernel call? Which files need to be changed?
> 5. (**MINIX**) What do MINIX 3.2.1 user space and kernel space consist of? Where can you find the corresponding code for the kernel/user space?
> 6. (**Kernel**) What are kernel arguments (kernel parameters)? How are they used in Linux and MINIX? What are MINIX3 boot options and when are they used?
>
> Each group can select one topic from the above 12 topics. You can take a glance now, and once the new groups are formed, please discuss with your new group members about which topic to choose. 
>
> At least one of the pair members has to show up during the presentation. For those who have trouble attending the lab sessions on Thursday, try to help your partner to find information and prepare the slides. You can check the recordings afterwards. 
>
> If you have any questions, feel free to contact us via Piazza or email.

#### [P3] Meet your new colleagues (Nov 23)

> Dear CoolZoners,
>
> Welcome aboard! At CoolZone IT solutions we are committed to providing a supportive environment to learn and grow; as such we hope you will find your experience enjoyable and rewarding. 
>
> Based on your background checks on your performance in Lemonion Inc., we have divided you into different p3-groups. Please find your new colleagues, and each group will be assigned a Git repository soon. Jim expects to see your work organized on Git, so all of you are suggested to make good use of it. 
>
> We are also aware that you have formed coding pairs in Lemonion Inc. and have adjusted some of them according to our background checks, especially the pairs randomly assigned originally. For the future lab sessions (starting from lab9), you will work with your new p3-group-pair partner(s).
>
> Lastly, some good news for those of you who were too busy when leaving Lemonion Inc. Thanks to a new collaboration project between Lemonion Inc. and us, the papers of the Lemonion Employee Evaluation Examination (midterm) will be made available during the paper checking session of the CoolZone IT Employee Evaluation Examination (final).
>
> Yours sincerely,
>
> Sylvia
>
> Human Resources Manager, CoolZone IT solutions
>
> [sylvia1993@coolzone.com](mailto:sylvia1993@coolzone.com) T: +86 21 29482503 | C: +86 13903121623

#### [P3] The end is nigh (Dec 9)

> Dear CoolZoners,
>
> Hope you are having fun on Jim's project. After it is due, we will be organizing a final delivery on the project on Nov. 16 from 11:30 to 15:30 in room F114, where Jim will take a look at your work. Two senior engineers (TAs) will be present, while another will oversee the process online. Each group will sign up for a 20-minute time slot in *Project 3 Final Delivery* in the "Scheduler" Tab under the "Calendar" section on Canvas. All the group members are expected to be physically in the room to present, so choose a slot when all of you are free. 
>
> Your Git repositories will be archived at 23:59 on Nov.15. On the next day, all the team members are expected to show up at least 10 minutes earlier. This time will be used to pull the code from the Git repository and compile it, in the presence of a senior engineer. During the final delivery of your project, you will need to present your MINIX source code (instead of git commits): which files have you modified, which part of code have you added, and your test program. 
>
> Please contact Jim or one of the three senior engineers if you meet any problems.
>
> Cheers,
>
> CoolZone IT Solutions Operations Control Team

#### [P2] Payday (Dec 26)

> Dear Lemonians,
>
> Thank you all for your hard work on fixing Lemondb. A round of pre-deployment testing has generated preliminary results on how well each team's version is doing. Based on these, we have formulated everyone's payrolls for this project, and you can find them on your Canvas account. The grading criteria is based on the one on the project manual, with three parts: documentation, correctness, and performance, in which part a zero was given for versions that are slower than the single-threaded one. 
>
> We strive to make Lemonion Inc. a fairer workplace for everyone. After consulting the Happy workplace initiative and looking at the number of commits for each group member, we have adjusted the actual paycheck everyone receives. If you find that your paycheck differs from the base pay, this is because we gave a penalty for the underworkers, a bonus for the workaholics, and a zero for those who barely worked. 
>
> If you have any problems for your paycheck, do not hesitate to contact us before **11:59 A.M., Dec. 27**. After this deadline, we will work with the CoolZone IT Solutions Operations Control Team to finalize a rating for your performance throughout the past three months, and any changes in the paychecks for any of the projects, labs, or assignments will no longer be available.
>
> Best regards,
>
> Lemonion Inc. Operations Control Team
