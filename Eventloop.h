#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>

#include "Channel.h"
#include "Poller.h"

class Eventloop
{
private:
    /* data */
    typedef std::function<void ()> Functor;
    typedef std::vector<Channel *> ChannelList;
    
    // 正在looping
    bool looping_;
    // 正在处理事件
    bool eventHandling;
    // 正在执行挂起函数
    bool doingPendingFunctor;
    // 退出loop
    bool quit_;
    // 互斥量
    std::mutex mutex_;

    // 具体执行poll的Poller
    std::unique_ptr<Poller> poller_;

    // 当前活跃的Channels
    ChannelList activeChannels_;
    // 挂起的函数
    std::vector<Functor> pendingFunctors_;

public:
    Eventloop(/* args */);
    ~Eventloop();

    // poller进行loop
    void loop();
    // 退出loop
    void quit();
    // 更新Channel设置
    void updateChannel(Channel *channel);
    // 移除Channel
    void removeChannel(Channel *channel);
    // 执行挂起函数
    void dopendingFunctor();
    // 加入挂起函数等待队列
    void addInPendingFunctors()
};


#endif