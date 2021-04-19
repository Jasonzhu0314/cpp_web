#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <vector>


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
    

private:
    typedef std::vector<std::thread*> Threads;
    Threads m_threads_;
    int thread_nums_;

    void ThreadLoop();

};