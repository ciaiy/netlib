#include "EventloopThreadPool.h"
#include <assert.h>

EventloopThreadPool::EventloopThreadPool(Eventloop* baseLoop, const std::string& nameArg)
  : baseLoop_(baseLoop),
    name_(nameArg),
    started_(false),
    numThreads_(4),
    next_(0)
{
    start();
}

void EventloopThreadPool::start(const ThreadInitCallBack& cb) {
    assert(!started_);
    std::cout << "threadpool start!" << std::endl;
    started_ = true;
    for(int i = 0; i < numThreads_; i++) {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof(buf), "%s%d", name_.c_str(), i);
        EventloopThread* t = new EventloopThread(cb, buf);
        threads_.push_back(std::unique_ptr<EventloopThread>(t));
        loops_.push_back(t->startLoop());
    }
    if(numThreads_ == 0 && cb) {
        cb(baseLoop_);
    }
}

Eventloop* EventloopThreadPool::getNextLoop() {
    Eventloop* loop = baseLoop_;
    if(!loops_.empty()) {
        loop = loops_[next_];
        ++next_;
        if(next_ >= loops_.size()) {
            next_ = 0;
        }
    }
    return loop;
}