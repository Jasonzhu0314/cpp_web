#include "thread_pools.h"


ThreadPool::ThreadPool(int num):
            thread_nums_(num)
{
    
}

ThreadPool::~ThreadPool() {
    if (is_started_) {
        stop();
    }
}

void ThreadPool::start() {
    is_started_ = true;
    threads_.reserve(thread_nums_);
    for (int i = 0; i < thread_nums_; i++) {
        threads_.push_back(new std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (is_started_) {
        int accept_fd;
        Task task = getTask(accept_fd);

        if (task) {
            task(accept_fd);
        }
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        is_started_ = false;
        cond_.notify_all();
    }
    is_started_ = false;
    for (auto it=threads_.begin(); it != threads_.end(); it++) {
       (*it)->join();
       delete *it; 
    } 
    threads_.clear();
}

void ThreadPool::addTask(const Task& task, int accept_fd) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(task);
    accept_list_.push_back(accept_fd);
    cond_.notify_one();
}

ThreadPool::Task ThreadPool::getTask(int &accept_fd) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (tasks_.empty() && is_started_) {
        cond_.wait(lock);
    }

    Task task = nullptr;
    // 当调用stop时，队列中可能为空
    if (!tasks_.empty()) {
        task = tasks_.front();
        tasks_.pop_front();
    }
    if (!accept_list_.empty()) {
        accept_fd = accept_list_.front();
        accept_list_.pop_front();
    }
    return task;
}
