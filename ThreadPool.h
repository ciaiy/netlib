#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

typedef std::function<void ()> taskCallBack;

class ThreadPool
{
private:
    /* data */
    std::vector<taskCallBack> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::vector<std::thread *> threads_;

    /* 禁用拷贝函数 */
    ThreadPool(ThreadPool &) = delete;
    const ThreadPool& operator= (ThreadPool &) = delete; 

    bool started_;
    int threadNum_;

    void threadloop();
    taskCallBack takeTask();
public:
    ThreadPool(int threadNum = 4);
    void addTask(taskCallBack cb);
    void stop();
    ~ThreadPool();
};



#endif // !THREADPOLL_H