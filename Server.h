#ifndef SERVER_H
#define SERVER_H

#include "Poller.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "CallBacks.h"

#include <map>
#include <iostream>
#include <functional>

namespace {
    typedef std::function<void (int newconfd)> newConnectionCallBack;
}

class Server
{
private:
    /* data */
    Acceptor accpetor_;
    Eventloop loop_;

    std::map<int, TcpConnection*> TcpConnections_;
public:
    Server(int port, char *address);
    void setNewConnectionCallBack(newConnectionCallBack cb);
    void newConnection(int sockfd);
    void start();
    ~Server();
};


#endif