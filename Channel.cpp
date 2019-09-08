#include "Channel.h"

/* debug */
#include <iostream>

void Channel::update() {
log(DEBUG, "channel", __LINE__, "update begin");
    loop_->updateChannel(this);
log(DEBUG, "channel", __LINE__, "update end");
}

void Channel::remove() {
    loop_->removeChannel(this);
}

void Channel::handleEvent()
{
    log(DEBUG, "Channel", __LINE__, "handleEvent begin");
    eventHanding_ = true;
    // 关闭连接
    // 等待验证是否为 !(revents_&EPOLLIN)
    if ((revents_ & EPOLLHUP ) || (revents_ & EPOLLRDHUP))
    {

    log(DEBUG, "Channel", __LINE__, "handleEvent:closeCallBacks");
        if (closeCallBack_)
        {
            closeCallBack_();
        }
    }

    // 读数据到来
    if(revents_ & (EPOLLIN | EPOLLPRI)) {
    log(DEBUG, "Channel", __LINE__, "handleEvent:readCallBacks");
        if(readCallBack_) {
            readCallBack_();
        }
    }

    // 写数据到来
    if(revents_ & (EPOLLOUT)) {
    log(DEBUG, "Channel", __LINE__, "handleEvent:ReadCallBacks");
        if(writeCallBack_) {
            writeCallBack_();
        }
    }

    // 出错
    if(revents_ & EPOLLERR) {
    log(DEBUG, "Channel", __LINE__, "handleEvent:ErrorCallBacks");
        if(errorCallBack_) {
            errorCallBack_();
        }
    }

    eventHanding_ = false;
}