#ifndef EVENTLOOP_THREAD
#define EVENTLOOP_THREAD

#include <iostream>
#include <functional>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class Eventloop;

class EventloopThread
{
public:
    typedef std::function<void(Eventloop *)> ThreadInitCallBack;

    EventloopThread(const ThreadInitCallBack &cb = ThreadInitCallBack(),
                    const std::string &name = std::string());
    ~EventloopThread();
    Eventloop *startLoop();
private:
    void threadFunc();

    Eventloop* loop_;
    bool exiting_;
    std::thread *thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallBack callback_;

};

#endif // !EVENTLOOP_THREAD
