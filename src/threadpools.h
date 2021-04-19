#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <vector>
#include <condition_variable>
#include <deque>

namespace zhuzs {

};

class ThreadPool {
public:
    typedef std::function<void()> Task;
    
    ThreadPool();
    ThreadPool(int num);
    ThreadPool(const ThreadPool &thread_pool) = delete;
    ThreadPool& operator=(const ThreadPool &thread_pool) = delete;
    ~ThreadPool();

    void start();
    void stop();
    void addTask(const Task&);
    Task get();
    

private:
    typedef std::vector<std::thread*> Threads;
    Threads threads_;
    int thread_nums_;
    bool is_started_;
    std::mutex mutex_;
    std::deque<Task> tasks_;
    std::condition_variable cond_;

    void ThreadLoop();

};