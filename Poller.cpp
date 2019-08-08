#include "Poller.h"

using std::cout;
using std::endl;

namespace
{
#define KNEW 1
#define KDELETED 2
#define KADDED 3
} // namespace

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

void Poller::loop(ChannelList *activeChannels_)
{
    int eventnum = epoll_wait(epollfd_,
                              &*revents_.begin(),
                              static_cast<int>(revents_.size()),
                              timeoutMs);
    if (eventnum > 0)
    {
        cout << "有" << eventnum << "个事件发生" << endl;
        fillactiveChannels(eventnum, activeChannels_);
        if (eventnum == revents_.size())
        {
            revents_.resize(eventnum * 2);
        }
    }
    else
    {
        cout << "无事件发生" << endl;
    }
}

void Poller::fillactiveChannels(int eventnum, ChannelList *activeChannels_)
{
    for (int i = 0; i < eventnum; i++)
    {
        Channel *channel = static_cast<Channel *>(revents_[i].data.ptr);
        int fd = channel->getFd();
        channels_[fd]->setRevent(revents_[i].events);
        activeChannels_->push_back(channel);
    }
}

void Poller::updateChannel(Channel *channel)
{
    // 未被epoll监听的channel（新的/被删除的）
    if (channel->getIndex() == KNEW || channel->getIndex() == KDELETED)
    {
        if (channel->getIndex() == KNEW)
        {
            // 加入到channels_
            channels_[channel->getFd()] = channel;
        }
        channel->setIndex = KADDED;
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        if (channel->isNoEvent())
        {
            // 删除epoll监听事件
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(KDELETED);
        }
        else
        {
            // 修改epoll监听事件
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Poller::removeChannel(Channel *channel)
{
    // 从channels_中删除， 如果状态为已监听， 则删除epoll监听事件
    channel->setIndex(KDELETED);
    channels_.erase(channel->getFd());
    if (channel->getIndex() == KADDED)
    {
        update(EPOLL_CTL_DEL, channel);
    }
}

void Poller::update(int type, Channel *channel)
{
    struct epoll_event ev = {0};
    ev.events = channel->getEvents();
    ev.data.ptr = static_cast<void *>(channel);
    epoll_ctl(epollfd_, type, channel->getFd(), &ev);
}