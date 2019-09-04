#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Socket.h"
#include "Channel.h"

#include <functional>
#include <iostream>

using std::string;

namespace
{
typedef std::function<void(int clientfd)> newConnectionCallBack;
};

class Channel;
class Acceptor
{
private:
    /* data */
    int idlefd;
    Socket socket_;
    Eventloop *loop_;
    Channel channel_;
    newConnectionCallBack newConnectionCallBack_;
    void handleRead(); // channel_的读回调函数

public:
    Acceptor(Eventloop *loop, int port, string address);
    ~Acceptor();
    Channel *getChannel() {
        return  &channel_;
    }
    void setNewConnectionCallBack(newConnectionCallBack cb);
};

#endif