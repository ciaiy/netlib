#include "Channel.h"

/* debug */
#include <iostream>

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::remove() {
    loop_->removeChannel(this);
}

void Channel::handleEvent()
{

    // 关闭连接
    log("INFO", "Channel", __LINE__, "channelfd:", fd_,"recive event:", revents_);
    if ((revents_ & EPOLLHUP ) || (revents_ & EPOLLRDHUP))
    {
        if (closeCallBack_)
        {
            closeCallBack_();
        }
        return ;
    }

    // 读数据到来
    if(revents_ & (EPOLLIN | EPOLLPRI)) {
        if(readCallBack_) {
            readCallBack_();
        }
        return ;
    }

    // 写数据到来
    if(revents_ & (EPOLLOUT)) {
        if(writeCallBack_) {
            writeCallBack_();
        }
        return ;
    }

    // 出错
    if(revents_ & EPOLLERR) {
        if(errorCallBack_) {
            errorCallBack_();
        }
        return ;
    }

    eventHanding_ = false;
}