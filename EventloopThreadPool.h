#ifndef EVENT_LOOP_THREAD_POOL
#define EVENT_LOOP_THREAD_POOL

#include "Eventloop.h"
#include "EventloopThread.h"

#include <iostream>
#include <functional>
#include <vector>

class EventloopThreadPool
{ 
    public:
    typedef std::function<void (Eventloop*)> ThreadInitCallBack;

    EventloopThreadPool(Eventloop *baseLoop, const std::string &nameArg);
    void start(const ThreadInitCallBack& cb = ThreadInitCallBack());
    Eventloop* getNextLoop();
    bool started() {return started_;}
    std::string name() {return name_;}
private:
    Eventloop * baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::string name_;
    std::vector<std::unique_ptr<EventloopThread> > threads_;
    std::vector<Eventloop *> loops_;
};



#endif