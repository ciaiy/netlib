#ifndef POLLER_H
#define POLLER_H

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>

#include "Eventloop.h"
#include "Channel.h"


class Channel;
class Eventloop;

class Poller
{
private:
    /* data */
    typedef std::vector<Channel *> ChannelList;
    typedef std::map<int, Channel*> channelMap;
    typedef std::vector<struct epoll_event> eventList;
    
    Eventloop *loop_;
    channelMap channels_;
    int epollfd_;
    eventList revents_;

public:
    int timeoutMs;
    Poller(Eventloop *loop_);
    void poll(ChannelList *activeChannels_);
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    void fillactiveChannels(int eventnum, ChannelList *acticeChannels_);
    void update(int type, Channel *channel);

/* debug */
    channelMap getchannels() {
        return channels_;
    }    
    ~Poller();
};

#endif