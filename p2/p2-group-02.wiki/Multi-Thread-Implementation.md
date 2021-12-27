# Multi-Thread Implementation
## Thread Pool
* Basic Usage
``` cpp
std::vector<future<T>> ress;
ThreadPool pool(size); // specifying the size of the pool
for (std::size_t i=1;i<size;i++){
    auto res = pool.enqueue(forwarder,args);// forward is the function; args is its arguments
    ress.emplace_back(res.get());
}
```
Tips:
* No need to worry about `future` a `get` will make it the `T` type such as `int` as you wished.
* Currently no efficiency is tested yet.
## Pop-up Thread
The arrival of message causes the system to create a new thread to handle the message.