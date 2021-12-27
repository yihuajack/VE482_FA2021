# Test for Lottery Scheduling
## Test Program
```
exit flag <- false
i <- 0
counter <- 0
nice_val <- 2
while (i < 10)
    pid = fork();
    if (pid)
        // parent
        nice(deadline = nice_val)
    else
        // child
        break
    end
    i <- i + 1
end
signal(SIGINT, signal_cb)
if (i == 10)
    // parent
    sleep for 5 seconds
    kill parent by SIGINT
end
while (exit flag)
    counter <- counter + 1
```
## Sample Test Results
```
Lottery testcase
proc:160	 the 10 most tickets is created
proc:161	 the 9 most tickets is created
proc:162	 the 8 most tickets is created
proc:163	 the 7 most tickets is created
proc:164	 the 6 most tickets is created
proc:165	 the 5 most tickets is created
proc:166	 the 4 most tickets is created
proc:167	 the 3 most tickets is created
proc:168	 the 2 most tickets is created
proc:169	 the 1 most tickets is created
pid:167	 the 3 most tickets 	time:88
pid:162	 the 8 most tickets 	time:259
pid:166	 the 4 most tickets 	time:116
pid:165	 the 5 most tickets 	time:154
pid:160	 the 10 most tickets 	time:227
pid:163	 the 7 most tickets 	time:212
pid:164	 the 6 most tickets 	time:233
pid:168	 the 2 most tickets 	time:94
pid:161	 the 9 most tickets 	time:216
pid:169	 the 1 most tickets 	time:61
```