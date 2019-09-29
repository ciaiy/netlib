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
    // 防止多线程同时操作同一socket, 出现不可预知的错误
    // test me :  是否还会出现竞争? 是否需要互斥量  // 正在尝试加入互斥量
    if(isEventHanding()) {
        return;
    }

    // 关闭连接
    // 等待验证是否为 !(revents_&EPOLLIN)
    if ((revents_ & EPOLLHUP ) || (revents_ & EPOLLRDHUP))
    {

        printf("DEBUG, Channel, fd : %d, closeCallBack\n", getFd());
        if (closeCallBack_)
        {
            loop_->addInPendingFunctors(std::bind(&Channel::closeCallBack_, this));
        }
        return ;
    }

    // 读数据到来
    if(revents_ & (EPOLLIN | EPOLLPRI)) {
    printf("DEBUG, Channel, fd : %d, readCallBack\n", getFd());
        if(readCallBack_) {
            readCallBack_();
        }
        return ;
    }

    // 写数据到来
    if(revents_ & (EPOLLOUT)) {
    printf("DEBUG, Channel, fd : %d, writeCallBack\n", getFd());    
        if(writeCallBack_) {
            writeCallBack_();
        }
        return ;
    }

    // 出错
    if(revents_ & EPOLLERR) {
    printf("DEBUG, Channel, fd : %d, errorCallBack\n", getFd());
        if(errorCallBack_) {
            errorCallBack_();
        }
        return ;
    }

    eventHanding_ = false;
}