# Monitor

A monitor is a synchronization construct that allows threads to have both mutual exclusion and the ability to wait (block) for a certain condition to become false.

A monitor consists of a mutex (lock) object and condition variables.

```
class Account {
    private lock myLock

    private int balance := 0
    invariant balance >= 0

    public method boolean withdraw(int amount)
       precondition amount >= 0
    {
        myLock.acquire()
        try {
            if balance < amount {
                return false
            } else {
                balance := balance - amount
                return true
            }
        } finally {
            myLock.release()
        }
    }

    public method deposit(int amount)
       precondition amount >= 0
    {
        myLock.acquire()
        try {
            balance := balance + amount
        } finally {
            myLock.release()
        }
    }
}
```

