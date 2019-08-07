#include "Poller.h"

using std::endl;
using std::cout;

Poller::Poller(Eventloop *loop_)
    : loop_(loop_),
      epollfd_(epoll_create1(EPOLL_CLOEXEC)),
      timeoutMs(100)
{
    if (epollfd_ == -1)
    {
        perror("in Poller()");
    }
}

Poller::~Poller()
{
    close(epollfd_);
}

void Poller::loop(ChannelList *activeChannels_) {
    int num = epoll_wait(epollfd_,
                        &*revents_.begin(),
                        static_cast<int>(revents_.size()),
                        timeoutMs);
    if(num > 0) {
        cout << "有" << num << "个事件发生" <<endl;
        fillactiveChannels(revents_);
        if(num == revents_.size()) {
            revents_.resize(num * 2);
        }
    }else {
        cout << "无事件发生" << endl;
    }
}

void Poller::fillactiveChannels(eventList revents_) {
    
}
