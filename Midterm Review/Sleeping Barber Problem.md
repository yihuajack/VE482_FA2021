# Sleeping Barber Problem

The analogy is based upon a hypothetical barber shop with one barber. The barber has one barber's chair in a cutting room and a waiting room containing a number of chairs in it. When the barber finishes cutting a customer's hair, he dismisses the customer and goes to the waiting room to see if there are others waiting. If there are, he brings one of them back to the chair and cuts their hair. If there are none, he returns to the chair and sleeps in it.

Each customer, when they arrive, looks to see what the barber is doing. If the barber is sleeping, the customer wakes him up and sits in the cutting room chair. If the barber is cutting hair, the customer stays in the waiting room. If there is a free chair in the waiting room, the customer sits in it and waits their turn. If there is no free chair, the customer leaves.

Based on a naïve analysis, the above decisions should ensure that the shop functions correctly, with the barber cutting the hair of anyone who arrives until there are no more customers, and then sleeping until the next customer arrives. In practice, there are a number of problems that can occur that are illustrative of general scheduling problems.

The problems are all related to the fact that the actions by both the barber and the customer (checking the waiting room, entering the shop, taking a waiting room chair, etc.) all take an unknown amount of time. For example, a customer may arrive and observe that the barber is cutting hair, so he goes to the waiting room. While they're on their way, the barber finishes their current haircut and goes to check the waiting room. Since there is no one there (maybe the waiting room is far away and the barber walks faster passing the customer, or maybe the customer went to the restroom or was going towards the chair and the barber thought he was leaving), so the barber goes back to their chair and sleeps. The barber is now waiting for a customer, but the customer is waiting for the barber.

In another situation, two customers may arrive at the same time when there happens to be a single seat in the waiting room. They observe that the barber is cutting hair, go to the waiting room, and both attempt to occupy the single chair.

```python
# The first two are mutexes (only 0 or 1 possible)
Semaphore barberReady = 0
Semaphore accessWRSeats = 1     # if 1, the number of seats in the waiting room can be incremented or decremented
Semaphore custReady = 0         # the number of customers currently in the waiting room, ready to be served
int numberOfFreeWRSeats = N     # total number of seats in the waiting room

def Barber():
  while true:                   # Run in an infinite loop.
    wait(custReady)             # Try to acquire a customer - if none is available, go to sleep.
    wait(accessWRSeats)         # Awake - try to get access to modify # of available seats, otherwise sleep.
    numberOfFreeWRSeats += 1    # One waiting room chair becomes free.
    signal(barberReady)         # I am ready to cut.
    signal(accessWRSeats)       # Don't need the lock on the chairs anymore.
    # (Cut hair here.)

def Customer():
  while true:                   # Run in an infinite loop to simulate multiple customers.
    wait(accessWRSeats)         # Try to get access to the waiting room chairs.
    if numberOfFreeWRSeats > 0: # If there are any free seats:
      numberOfFreeWRSeats -= 1  #   sit down in a chair
      signal(custReady)         #   notify the barber, who's waiting until there is a customer
      signal(accessWRSeats)     #   don't need to lock the chairs anymore
      wait(barberReady)         #   wait until the barber is ready
      # (Have hair cut here.)
    else:                       # otherwise, there are no free seats; tough luck --
      signal(accessWRSeats)     #   but don't forget to release the lock on the seats!
      # (Leave without a haircut.)
```

```c
Semaphore Customers = 0;
Semaphore Barber = 0;
Mutex Seats = 1;
int FreeSeats = N;

Barber {
	while(true) {
			/* waits for a customer (sleeps). */
			down(Customers);

			/* mutex to protect the number of available seats.*/
			down(Seats);

			/* a chair gets free.*/
			FreeSeats++;
			
			/* bring customer for haircut.*/
			up(Barber);
			
			/* release the mutex on the chair.*/
			up(Seats);
			/* barber is cutting hair.*/
	}
}

Customer {
	while(true) {
			/* protects seats so only 1 customer tries to sit
			in a chair if that's the case.*/
			down(Seats); //This line should not be here.
			if(FreeSeats > 0) {
				
				/* sitting down.*/
				FreeSeats--;
				
				/* notify the barber. */
				up(Customers);
				
				/* release the lock */
				up(Seats);
				
				/* wait in the waiting room if barber is busy. */
				down(Barber);
				// customer is having hair cut
			} else {
				/* release the lock */
				up(Seats);
				// customer leaves
			}
	}
}
```

Our solution uses three semaphores, customers, which counts waiting customers  (excluding the customer in the barber chair, who is not waiting), barbers, the number of  barbers (0 or 1) who are idle, waiting for customers, and mutex, which is used for mutual  exclusion. We also need a variable, waiting, which also counts the waiting customers.  The reason for having waiting is that there is no way to read the current value of a  semaphore. In this solution, a customer entering the shop has to count the number of  waiting customers. If it is less than the number of chairs, he stays; otherwise, he leaves.

```c
#define CHAIRS 5 /* # chairs for waiting customers */
typedef int semaphore; /* use your imagination */
semaphore customers = 0; /* # of customers waiting for service */
semaphore barbers = 0; /* # of barbers waiting for customers */
semaphore mutex = 1; /* for mutual exclusion */
int waiting = 0; /* customer are waiting (not being cut) */
void barber(void)
{
 while (TRUE) {
 down(&customers); /* go to sleep if # of customers is 0 */
 down(&mutex); /* acquire access to "waiting' */
 waiting = waiting - 1; /* decrement count of waiting customers */
 up(&barbers); /* one barber is now ready to cut hair */
 up(&mutex); /* release 'waiting' */
 cut_hair(); /* cut hair (outside critical region */
 }
}
void customer(void)
{
 down(&mutex); /* enter critical region */
 if (waiting < CHAIRS) { /* if there are no free chairs, leave */
 waiting = waiting + 1; /* increment count of waiting customers */
 up(&customers); /* wake up barber if necessary */
 up(&mutex); /* release access to 'waiting' */
 down(&barbers); /* go to sleep if # of free barbers is 0 */
 get_haircut(); /* be seated and be served */
 } else {
 up(&mutex); /* shop is full; do not wait */
 }
}
```
