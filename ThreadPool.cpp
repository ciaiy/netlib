#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadNum) : threadNum_(threadNum),
                                        started_(true),
                                        cond_(),
                                        mutex_(),
                                        threads_(),
                                        tasks_()
{
    for (int i = 0; i < threadNum_; i++)
    {
        std::thread *th = new std::thread(std::bind(&ThreadPool::threadloop, this));
        threads_.push_back(th);
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::threadloop()
{
    while (started_)
    {
        taskCallBack task = takeTask();
        if (task)
        {
            task();
        }
    }
}

void ThreadPool::addTask(taskCallBack cb)
{
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(cb);
    /*test me : 当工作线程都在运行时, 可能会丢失任务*/
    cond_.notify_one();
}

taskCallBack ThreadPool::takeTask()
{
    /* 只有一个线程进入到临界区 */
    std::unique_lock<std::mutex> lock(mutex_);
    /* 判断是否任务列表为空 */
    while (tasks_.empty() && started_)
    {
        cond_.wait(lock);
    }
    taskCallBack cb;
    /* 排除任务队列为空或者线程池关闭 */
    if (!tasks_.empty() && started_)
    {
        cb = tasks_.back();
        tasks_.pop_back();
    }
        return cb;
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        started_ = false;
        cond_.notify_all();
    }

    for(std::vector<std::thread *>::iterator it = threads_.begin(); it != threads_.end(); it++) {
        (*it)->join();
        delete *it;
    }
    threads_.clear();
}