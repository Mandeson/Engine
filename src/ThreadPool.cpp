#include "ThreadPool.hpp"
#include <functional>
#include <mutex>
#include <cassert>

ThreadPool::ThreadPool() {
    threads_.reserve(kNumThreads);
    for (int i = 0; i < kNumThreads; i++) {
        threads_.emplace_back([this] {
            std::function<void()> func;
            while (true) {
                {
                    std::unique_lock lock{mutex_};
                    cond_.wait(lock, [this] {
                        return terminating_ || !jobs_.empty();
                    });
                    if (terminating_ && jobs_.empty())
                        return;
                    func = jobs_.front();
                    jobs_.pop();
                }
                func();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    if (terminating_)
        return;
    terminate();
}

void ThreadPool::execute(std::function<void()> func) {
    {
        std::unique_lock lock{mutex_};
        jobs_.push(func);
    }
    cond_.notify_one();
}

void ThreadPool::terminate() {
    {
        std::unique_lock lock{mutex_};
        terminating_ = true;
    }
    cond_.notify_all();
    for (auto &thread : threads_)
        thread.join();
}
