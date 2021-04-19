#include "threadpools.h"


ThreadPool::ThreadPool(int num):thread_nums_(num)
{

}

ThreadPool::~ThreadPool() {

}

void ThreadPool::start() {
    for (int i = 0; i < thread_nums_; i++) {
        m_threads_.push_back(new std::thread(ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {

}