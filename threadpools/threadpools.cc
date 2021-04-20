#include "threadpools.h"


ThreadPool::ThreadPool(int num):
            thread_nums_(num),
            is_started_(false)
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
        threads_.push_back(new std::thread(ThreadPool::ThreadLoop, this));
    }
}
void ThreadPool::ThreadLoop() {
    while (is_started_) {
        Task task = get();
        if (task) {
            task();
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

void ThreadPool::addTask(const Task& task) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(task);
    cond_.notify_one();
}

ThreadPool::Task ThreadPool::get() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (tasks_.empty() && is_started_) {
        cond_.wait(lock);
    }

    Task task = nullptr;
    // 当调用stop时，队列中可能为空
    if (!tasks_.empty()) {
        tasks_.front();
        tasks_.pop_front();
    }
    return task;
}

