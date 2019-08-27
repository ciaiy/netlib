#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <sys/epoll.h>

#include "Eventloop.h"

namespace
{
#define KNEW 1
#define KDELETED 2
#define KADDED 3

#define EVENT_READ 1
#define EVENT_WRITE 2

typedef std::function<void()> readCallBack;
typedef std::function<void()> writeCallBack;
typedef std::function<void()> closeCallBack;
typedef std::function<void()> errorCallBack;

} // namespace

class Channel
{
private:
    /* data */
    int fd_;
    Eventloop *loop_;

    // 正在处理事件
    bool eventHanding_;

    // 此时channel的状态
    int status_;
    // epoll监听到的事件类型
    unsigned int revents_;
    // epoll要监听的事件类型
    unsigned int events_;

    // 回调函数
    readCallBack readCallBack_;
    writeCallBack writeCallBack_;
    closeCallBack closeCallBack_;
    errorCallBack errorCallBack_;

public:
    Channel(Eventloop *loop, int fd)
        : loop_(loop),
          fd_(fd), 
          revents_(0), 
          events_(0) 
    {
    };
    
    ~Channel();
    void handleEvent();
    void remove();
    void update();

    // 开关读写函数

    void disableAll()
    {
        events_ = 0;
        update();
    }

    void enableRead()
    {
        events_ |= EVENT_READ;
        update();
    }

    void disableRead()
    {
        events_ &= ~EVENT_READ;
        update();
    }

    void enableWrite()
    {
        events_ |= EVENT_WRITE;
        update();
    }

    void disableWrite()
    {
        events_ &= ~EVENT_WRITE;
        update();
    }

    // 验证类函数

    bool isEventHanding()
    {
        return eventHanding_;
    }

    bool isNoEvent()
    {
        return events_ == 0;
    }

    // get* 和 set* 函数
    int getFd()
    {
        return fd_;
    }
    int getStatus()
    {
        return status_;
    }

    void setStatus(int status)
    {
        status_ = status;
    }

    unsigned int getEvents()
    {
        return events_;
    }

    void setRevents(unsigned int revents)
    {
        revents_ = revents;
    }

    void setReadCallBack(readCallBack cb)
    {
        readCallBack_ = std::move(cb);
    }

    void setWriteCallBack(writeCallBack cb)
    {
        writeCallBack_ = std::move(cb);
    }

    void setCloseCallBack(closeCallBack cb)
    {
        closeCallBack_ = std::move(cb);
    }

    void setErrorCallBack(errorCallBack cb)
    {
        errorCallBack_ = std::move(cb);
    }
};

#endif