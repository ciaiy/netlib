#include "EventloopThread.h"
#include "Eventloop.h"

void Init(Eventloop* loop) {
    std::cout << "thread start! loop" << loop <<std::endl;
}

EventloopThread::EventloopThread(const ThreadInitCallBack&cb,
                                 const std::string& name)
                                :loop_(NULL),
                                exiting_(false),
                                mutex_(),
                                cond_(),
                                callback_(cb)
{
    loop_ = new Eventloop();
}

EventloopThread::~EventloopThread() {
    exiting_ = true;
    if(loop_ != NULL) {
        loop_->quit();
        thread_->join();
        delete thread_;
    }
}



Eventloop* EventloopThread::startLoop() {
    thread_ = new std::thread(std::bind(&EventloopThread::threadFunc, this));
    return loop_;
}

void EventloopThread::threadFunc() {
        std::cout << "start loop : " << std::this_thread::get_id() << std::endl;
    if(callback_) {
        callback_(loop_);
    }
    loop_->loop();
}