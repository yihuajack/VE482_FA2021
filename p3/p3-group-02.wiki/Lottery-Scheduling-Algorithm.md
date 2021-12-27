# Lottery Scheduling Algorithm
Initialize total tickets:
```
for queue q from USER_Q to NR_SCHED_QUEUES
    rp <- rdy_head[q]
    while (rp)
        if (rp is runnable)
            total tickets <- MIN_USER_Q - priority of rp
        end
        rp <- next ready rp
    end
end
```
Pick processes by lottery
```
if (total ticket > 0)
    randomize lucky
    for queue q from USER_Q to NR_SCHED_QUEUES
        rp <- rdy_head[q]
        while (rp)
            if (rp is runnable)
                ticket count <- ticket count + MIN_USER_Q - priority of rp
                if (ticket count > lucky)
                    bill for system time if billable
                    return rp
                end
            end
        end
    end
end
```
Pseudo-random number generator:
```
return (seed * 1103515245U + 12345U) & 0x7fffffff)
```