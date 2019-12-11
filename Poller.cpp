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
      timeoutMs(100),
      channels_(),
      revents_(100)
{
    if (epollfd_ == -1)
    {
        perror("in Poller()");
    }
    log("INFO", "Poller", __LINE__, "Poller construct", epollfd_, loop_);
}

Poller::~Poller()
{
    close(epollfd_);
}

void Poller::poll(ChannelList *activeChannels_)
{
    int eventnum = epoll_wait(epollfd_,
                              &(*revents_.begin()),
                              static_cast<int>(revents_.size()),
                              timeoutMs);
    if(eventnum == -1) {
        perror("epoll_wait error");
    }
    if (eventnum > 0)
    {
        // cout << "有" << eventnum << "个事件发生" << endl;
        fillactiveChannels(eventnum, activeChannels_);
        if (eventnum == revents_.size())
        {
            revents_.resize(eventnum * 2);
        }
    }
    else
    {
        // cout << "无事件发生" << endl;
    }
}

void Poller::fillactiveChannels(int eventnum, ChannelList *activeChannels_)
{
    for (int i = 0; i < eventnum; i++)
    {
        Channel *channel = static_cast<Channel *>(revents_[i].data.ptr);
        int fd = channel->getFd();
        log("DEBUG", "Poller", __LINE__, "fillactiveChannels", epollfd_, "channelfd", fd);
        channels_[fd]->setRevents(revents_[i].events);
        activeChannels_->push_back(channel);
    }
}

void Poller::updateChannel(Channel *channel)
{

    // 未被epoll监听的channel（新的/被删除的）
    if (channel->getStatus() == KNEW || channel->getStatus() == KDELETED)
    {
        if (channel->getStatus() == KNEW)
        {
            // 加入到channels_
            this->channels_[channel->getFd()] = channel;
        }
        channel->setStatus(KADDED);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        if (channel->isNoEvent())
        {
            // 删除epoll监听事件
            update(EPOLL_CTL_DEL, channel);
            channel->setStatus(KDELETED);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);

        }
    }
}

void Poller::removeChannel(Channel *channel)
{
    // 从channels_中删除， 如果状态为已监听， 则删除epoll监听事件
    // channels_.erase(channel->getFd());
    if (channel->getStatus() == KADDED)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setStatus(KDELETED);
}

void Poller::update(int type, Channel *channel)
{
    log("INFO", "Poller", __LINE__, "Poller update", "epoll_fd", epollfd_, "type", type, "channelfd", channel->getFd(), "events", channel->getEvents(), "status:", channel->getStatus(), "thread", std::this_thread::get_id());    
    struct epoll_event ev = {0};
    ev.events = channel->getEvents();
    ev.data.ptr = static_cast<void *>(channel);
    int ret_value = epoll_ctl(epollfd_, type, channel->getFd(), &ev);
    if(ret_value == -1) {
    log("ERROR", "Poller", __LINE__, "Poller update", "epoll_fd", epollfd_, "type", type, "channelfd", channel->getFd(), "events", channel->getEvents(), "status:", channel->getStatus(), "thread", std::this_thread::get_id());
        perror("epoll_ctl");
    }
}