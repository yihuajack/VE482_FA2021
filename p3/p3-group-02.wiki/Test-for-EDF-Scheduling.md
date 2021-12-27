# Test for EDF Scheduling
## Test Program
```
i <- 1
while (i < 5):
    if (fork() == 0)
        // parent
        switch i
            i = 1:
                chddl(15)
                sleep for 1 second
            i = 2:
                chddl(25)
                sleep for 1 second
            i = 3:
                chddl(35)
                sleep for 1 second
            i = 4:
                chddl(0)
                sleep for 1 second
        end
        loop <- 0
        while (loop < 30)
            switch i and loop
                i = 3 and loop = 5:
                    chddl(15)
                i = 1 and loop = 10:
                    chddl(30)
                i = 3 and loop = 15:
                    chddl(30)
            end
            sleep for 1 second
        end
        exit parent
    end
    i <- i + 1
```
## Expected Test Results
```
proc1 started
proc2 started
proc3 started
proc4 started
# prc4 tick 1
prc1 tick 1
prc2 tick 1
prc3 tick 1
prc4 tick 2
prc1 tick 2
prc2 tick 2
prc3 tick 2
prc4 tick 3
prc1 tick 3
prc2 tick 3
prc3 tick 3
prc4 tick 4
prc1 tick 4
prc2 tick 4
prc3 tick 4
Change proc3 deadline to 5+15s
prc4 tick 5
prc1 tick 5
prc3 tick 5
prc2 tick 5
prc4 tick 6
prc1 tick 6
prc3 tick 6
prc2 tick 6
prc4 tick 7
prc1 tick 7
prc3 tick 7
prc2 tick 7
prc4 tick 8
prc1 tick 8
prc3 tick 8
prc2 tick 8
prc4 tick 9
prc1 tick 9
prc3 tick 9
prc2 tick 9
Change proc1 deadline to 10+30s
prc4 tick 10
prc3 tick 10
prc2 tick 10
prc1 tick 10
prc4 tick 11
prc3 tick 11
prc2 tick 11
prc1 tick 11
prc4 tick 12
prc3 tick 12
prc2 tick 12
prc1 tick 12
prc4 tick 13
prc3 tick 13
prc2 tick 13
prc1 tick 13
prc4 tick 14
prc3 tick 14
prc2 tick 14
prc1 tick 14
Change proc3 deadline to 15+30s
prc4 tick 15
prc2 tick 15
prc1 tick 15
prc3 tick 15
prc4 tick 16
prc2 tick 16
prc1 tick 16
prc3 tick 16
prc4 tick 17
prc2 tick 17
prc1 tick 17
prc3 tick 17
prc4 tick 18
prc2 tick 18
prc1 tick 18
prc3 tick 18
prc4 tick 19
prc2 tick 19
prc1 tick 19
prc3 tick 19
prc4 tick 20
prc2 tick 20
prc1 tick 20
prc3 tick 20
prc4 tick 21
prc2 tick 21
prc1 tick 21
prc3 tick 21
prc4 tick 22
prc2 tick 22
prc1 tick 22
prc3 tick 22
prc4 tick 23
prc2 tick 23
prc1 tick 23
prc3 tick 23
prc4 tick 24
prc1 tick 24
prc3 tick 24
prc4 tick 25
prc1 tick 25
prc3 tick 25
prc4 tick 26
prc1 tick 26
prc3 tick 26
prc4 tick 27
prc1 tick 27
prc3 tick 27
prc4 tick 28
prc1 tick 28
prc3 tick 28
prc4 tick 29
prc1 tick 29
prc3 tick 29

```