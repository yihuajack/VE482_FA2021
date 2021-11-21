# Barrier

```c
struct barrier_type
{
    // how many processors have entered the barrier
    // initialize to 0
    int arrive_counter;
    // how many processors have exited the barrier
    // initialize to p
    int leave_counter;
    int flag;
    std::mutex lock;
};

// barrier for p processors
void barrier(barrier_type* b, int p)
{
    b->lock.lock();
    if (b->arrive_counter == 0)
    {
        b->lock.unlock();
        while (b->leave_counter != p); // wait for all to leave before clearing
        b->lock.lock();
        b->flag = 0; // first arriver clears flag
    }
    b->arrive_counter++;
    if (b->arrive_counter == p) // last arriver sets flag
    {
        b->arrive_counter = 0;
        b->leave_counter = 0;
        b->flag = 1;
    }
    b->lock.unlock();

    while (b->flag == 0); // wait for flag
    b->lock.lock();
    b->leave_counter++;
    b->lock.unlock();
}
```

Sense-Reversal Centralized Barrier:

```c
struct barrier_type
{
    int counter; // initialize to 0
    int flag; // initialize to 0
    std::mutex lock;
};

int local_sense = 0; // private per processor

// barrier for p processors
void barrier(barrier_type* b, int p)
{
    local_sense = 1 - local_sense;
    b->lock.lock();
    b->counter++;
    int arrived = b->counter;
    if (arrived == p) // last arriver sets flag
    {
        b->lock.unlock();
        b->counter = 0;
        // memory fence to ensure that the change to counter
        // is seen before the change to flag
        b->flag = local_sense;
    }
    else
    {
        b->lock.unlock();
        while (b->flag != local_sense); // wait for flag
    }
}
```

