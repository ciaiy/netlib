#include "Channel.h"

/* debug */
#include <iostream>

void Channel::update() {
/* debug */
std::cout << "channel update... fd" << fd_ << std::endl;
    loop_->updateChannel(this);
}

void Channel::remove() {
    loop_->removeChannel(this);
}

void Channel::handleEvent()
{
    eventHanding_ = true;
    // 关闭连接
    // 等待验证是否为 !(revents_&EPOLLIN)
    if (revents_ & EPOLLHUP && !(revents_ & EPOLLIN))
    {
        if (closeCallBack_)
        {
            closeCallBack_();
        }
    }

    // 读数据到来
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        if(readCallBack_) {
            readCallBack_();
        }
    }

    // 写数据到来
    if(revents_ & (EPOLLOUT)) {
        if(writeCallBack_) {
            writeCallBack_();
        }
    }

    // 出错
    if(revents_ & EPOLLERR) {
        if(errorCallBack_) {
            errorCallBack_();
        }
    }

    eventHanding_ = false;
}