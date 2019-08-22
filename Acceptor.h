#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "ServerSocket.h"
#include "Channel.h"

#include <functional>
#include <iostream>

using std::string;

namespace
{
typedef std::function<void(int clientfd)> newConnectionCallBack;
};

class Acceptor
{
private:
    /* data */
    int idlefd;
    Channel channel_;
    ServerSocket socket_;
    Eventloop *loop_;
    newConnectionCallBack newConnectionCallBack_;
    void handleRead(); // channel_的读回调函数

public:
    Acceptor(Eventloop *loop, int port, string address);
    ~Acceptor();

    void setNewConnectionCallBack(newConnectionCallBack cb);
};

Acceptor::Acceptor(Eventloop *loop, int port, string address)
    : loop_(loop),
      socket_(port, address),
      channel_(loop_, socket_.getsockfd())
{
    idlefd = open("/dev/null", O_RDONLY);
    if(idlefd == -1) {
        perror("idlefd open error");
        delete(this);
    }
    socket_.listen();
    channel_.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    channel_.disableAll();
    channel_.remove();
    close(idlefd);
}

void Acceptor::setNewConnectionCallBack(newConnectionCallBack cb)
{
    newConnectionCallBack_ = std::move(cb);
}

#endif