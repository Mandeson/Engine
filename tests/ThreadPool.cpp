#include "ThreadPool.h"
#include <atomic>
#include <iostream>

int main() {
    ThreadPool thread_pool;

    std::atomic_int v = 0;

    thread_pool.execute([&v] {
        for (int i = 0; i < 1000000; i++)
            v++;
    });
    thread_pool.execute([&v] {
        for (int i = 0; i < 2000000; i++)
            v++;
    });
    thread_pool.execute([&v] {
        for (int i = 0; i < 500000; i++)
            v++;
    });
    thread_pool.execute([&v] {
        for (int i = 0; i < 2000000; i++)
            v++;
    });
    thread_pool.execute([&v] {
        for (int i = 0; i < 2000000; i++)
            v++;
    });

    std::cout << v << std::endl;
}
