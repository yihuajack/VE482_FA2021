//
// Created by alaallen on 11/8/21.
//

#ifndef LEMONDB_THREADPOOL_H
#define LEMONDB_THREADPOOL_H
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "MultiThread.h"
#define DEL_SIZE 64
class ThreadPool {
private:
    std::vector< std::thread > threads;
    std::queue< std::function<void(int)> > tasks;
    std::mutex queueMutex;
    std::condition_variable cond;
    bool terminate;
    void infiniteLoop(int id);
    /**
    * A unique pointer to the global database object
    */
    static std::unique_ptr<ThreadPool> instance;
public:
    void start(){
        this->terminate = false;
    }
    explicit ThreadPool(size_t size);
    static ThreadPool &getInstance(); // must add static
    ~ThreadPool();
    template<class Func, typename ... Argv>
    auto enqueue(Func&& f, Argv&&... args)
    -> std::future<typename std::result_of<Func(Argv...)>::type>;
    template<class Func>
    std::future<void> enqueueTaskRequiresID(Func &&f);
    void closeAll();

};

inline std::unique_ptr<ThreadPool> ThreadPool::instance = nullptr;

inline ThreadPool & ThreadPool::getInstance() {
    if (ThreadPool::instance == nullptr) {
        long n_thread = getThreadNum();
        instance = std::unique_ptr<ThreadPool>(new ThreadPool((size_t )n_thread));
    }
    return *instance;
}

inline ThreadPool::ThreadPool(size_t size)
        :   terminate(false)
{
    threads.reserve(size);
    for(size_t i = 0;i<size;++i) {
        threads.emplace_back(
                [this, i]{ this->infiniteLoop((int) i);}
                // TODO debug here
        );//this->infiniteLoop);
    }
}


inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(this->queueMutex);
        this->terminate = true;
    }
    cond.notify_all();

    // wait for the unfinished tasks to finish
    for (auto &tmp:this->threads){
        tmp.join();
    }
    while (!this->tasks.empty())
        this->tasks.pop();
    this->threads.empty();
}

template<class Func, typename... Args>
auto ThreadPool::enqueue(Func &&f,Args&&... args)
-> std::future<typename std::result_of<Func(Args...)>::type>
{
    using return_type = typename std::result_of<Func(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
    //make shared for accessing else where
    std::future<return_type> res = task->get_future();
    //get future from packaged_task
    {
        std::unique_lock<std::mutex> lock(this->queueMutex);

        if (terminate){
            throw std::runtime_error("add new tasks to the pool after the pool is stopped");
        }

        tasks.emplace([task](int i){(*task)();});
        // TODO: need attention is that correct

    }
    cond.notify_one();
    //this->results.emplace(res);
    return res;
//    The thread that intends to modify the shared variable has to
//
//    acquire a std::mutex (typically via std::lock_guard)
//    perform the modification while the lock is held
//    execute notify_one or notify_all on the std::condition_variable (the lock does not need to be held for notification)
}

template<class Func>
std::future<void> ThreadPool::enqueueTaskRequiresID(Func &&f) {

    auto task = std::make_shared<std::packaged_task<void(int)>>(std::forward<Func>(f));
    //make shared for accessing else where
    std::future<void> res = task->get_future();
    //get future from packaged_task
    {
        std::unique_lock<std::mutex> lock(this->queueMutex);

        if (terminate){
            throw std::runtime_error("add new tasks to the pool after the pool is stopped");
        }

        tasks.emplace([task](int i){(*task)(i);});
        // TODO: need attention is that correct

    }
    cond.notify_one();
    //this->results.emplace(res);
    return res;
//    The thread that intends to modify the shared variable has to
//
//    acquire a std::mutex (typically via std::lock_guard)
//    perform the modification while the lock is held
//    execute notify_one or notify_all on the std::condition_variable (the lock does not need to be held for notification)
}


inline void ThreadPool::closeAll() {
//    {
//        std::unique_lock<std::mutex> lock(this->queueMutex);
//        this->terminate = true;
//    }
//    cond.notify_all();
//
//    // wait for the unfinished tasks to finish
//    for (auto &tmp:this->threads){
//        tmp.join();
//    }
    while (!this->tasks.empty())
        this->tasks.pop();
//    if (this->tasks.empty()){
//        throw std::runtime_error("close when tasks are not empty");
//    }
    //this->results.clear();
}

//template<typename T, class Reducer,typename F, class... Args>
//T ThreadPool<T, Reducer,F,Args...>::get(Reducer) {
//    // use the reducer to reduce vector<future<T>> to T
//    T res;
//    for (auto it: this->results){
//        res = Reducer(res,it.get());
//    }
//    return res;
//}





inline void ThreadPool::infiniteLoop(int i) {
    while (true){
        std::function<void(int)> task;
        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            // mutex ownership wrapper
            // this constructor automatically lock the mutex
            // this mutex is used to protect the condition in condition_variable
            this->cond.wait(lock, [this]{return this->terminate || !this->tasks.empty();});
            // condition_variable blocks this thread until
            // both
            // 1. condition is modified
            // 2. condition_variable is notified
            // from other threads
            // here the thread will be blocked until the condition_variable is notified and
            // the pool is terminated or the task queue is not empty, i.e. new task enqueued
            if (terminate && tasks.empty())
                return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task(i);
    }
}


#endif

