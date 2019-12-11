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
    // Eventloop* loop = NULL;
    // {
    //     std::unique_lock<std::mutex> lock(mutex_);
    //     while(loop_ == NULL) {
    //         cond_.wait(lock);
    //     }
    //     loop = loop_;
    // }
    // return loop;
}

void EventloopThread::threadFunc() {
        std::cout << "start loop : " << std::this_thread::get_id() << std::endl;
    if(callback_) {
        callback_(loop_);
    }
    loop_->loop();
    // {
    //     std::unique_lock<std::mutex> lock(mutex_);
    //     loop_ = &loop;
    //     cond_.notify_one();
    // }
    // loop.loop();
    // std::unique_lock<std::mutex> lock(mutex_);
    // loop_ = NULL;
}