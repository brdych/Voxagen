#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <iostream>
#include <condition_variable>
#include "vtask.hpp"

class ThreadPool {
private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<VTask*> tasks;
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPool(size_t threads):stop(false)
    {
        for(size_t i = 0;i<threads;++i)
            workers.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        VTask* task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock,[this]{ return this->stop || !this->tasks.empty(); });
                            if(this->stop && this->tasks.empty())
                            {
                                return;
                            }
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task->execute();
                        delete task;
                    }
                }
            );
    }

    void enqueue(VTask* task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            // don't allow enqueueing after stopping the pool
            if(!stop)
                tasks.emplace(task);
        }
        condition.notify_one();
    }

    ~ThreadPool()
    {
        std::cout << "Pool resetting!" << std::endl;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        std::cout << "Stop Order Issued!" << std::endl;
        condition.notify_all();
        std::cout << "Notified All, Waiting for Join...!" << std::endl;
        for(auto &worker: workers)
        {
            worker.join();
            std::cout << "A worker Joined!" << std::endl;
        }
        while(!tasks.empty())
        {
            VTask* t = tasks.front();
            tasks.pop();
            delete t;
        }
        std::cout << "Pool has reset!" << std::endl;
    }
};
#endif
