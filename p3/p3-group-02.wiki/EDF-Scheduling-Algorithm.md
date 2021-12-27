# EDF Scheduling Algorithm
The processes with deadline is put into `queue[USER_Q]` in `enqueue()` and `enqueue_head()`.
```
if queue q is USER_Q
    rp <- rdy_head[q]
    next runnable proc <- next ready rp
    while (next runnable proc)
        if (deadline of next runnable proc > 0 and (deadline of rp = 0 or deadline of rp > deadline of next runnable proc)
            rp <- next runnable proc
        end
        next runnable proc <- next ready rp
        end
end
bill for system time if billable
return rp
```