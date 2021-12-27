//
// Created by shenlang on 21-11-15.
//

#ifndef LEMONDB_LOCALREDUCER_H
#define LEMONDB_LOCALREDUCER_H

#include <mutex>
#include <vector>
#include <condition_variable>

#include "ThreadPool.h"
#include "MultiThread.h"

template<class T>
class LocalReducer {
private:
    int remaining_tasks;
    std::mutex tasks_mutex;
    std::condition_variable cond;
    std::vector<T> local_vars;
public:
    explicit LocalReducer(const T& t = T()): remaining_tasks(0), local_vars(getThreadNum(), t) {};
    ~LocalReducer() = default;
    template<class F>
    void enqueueTask(F f);
    void wait();
    std::vector<T>& get();
};

template<class T> template<class F>
void LocalReducer<T>::enqueueTask(F f) {
    ThreadPool& thread_pool = ThreadPool::getInstance();
    {
        std::unique_lock<std::mutex>{tasks_mutex};
        ++remaining_tasks;
    }
    thread_pool.enqueueTaskRequiresID([f, this](int i){
        f(this->local_vars[i]);
        {
            std::unique_lock<std::mutex>{this->tasks_mutex};
            this->remaining_tasks--;
        }
        this->cond.notify_one();
    });
}

template<class T>
void LocalReducer<T>::wait() {
    std::unique_lock<std::mutex> lock{this->tasks_mutex};
    cond.wait(lock, [this]{
        return this->remaining_tasks == 0;
    });
}

template<class T>
std::vector<T>& LocalReducer<T>::get() {
    wait();
    return local_vars;
}

#endif
