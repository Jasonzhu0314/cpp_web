#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <vector>
#include <condition_variable>
#include <deque>


const int kthread_nums = 3;

class ThreadPool {
public:
    //typedef std::function<void(int)> Task;
    typedef void (*Task) (int a);

    ThreadPool();
    ThreadPool(int num);
    ThreadPool(const ThreadPool &thread_pool) = delete;
    ThreadPool& operator=(const ThreadPool &thread_pool) = delete;
    ~ThreadPool();

    void start();
    void stop();
    void addTask(const Task&, int );
    Task getTask(int&);
    int thread_nums_;

private:
    bool is_started_;

    typedef std::vector<std::thread*> Threads;
    Threads threads_;
    
    std::mutex mutex_;
    std::deque<Task> tasks_;
    std::deque<int> accept_list_;
    std::condition_variable cond_;

    void ThreadLoop();

};
