#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>

class ThreadPool {
public:
    ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    ~ThreadPool();
    void execute(std::function<void()> func);
    void terminate();
private:
    static constexpr int kNumThreads = 4;

    std::queue<std::function<void()>> jobs_;
    std::vector<std::thread> threads_;
    bool terminating_{false};
    std::mutex mutex_;
    std::condition_variable cond_;
};
