#ifndef SERVER_H
#define SERVER_H

#include "Poller.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "defaultCallBacks.h"
#include "logger.h"

#include <map>
#include <iostream>
#include <functional>

class TcpConnection;
class Acceptor;

namespace {
    typedef std::function<void (int newconfd)> newConnectionCallBack;
}

class Server
{
private:
    /* data */
    Eventloop loop_;
    Acceptor accpetor_;
    ReadCompleteCallBack readCompleteCallBack_;
    ClosingCallBack closingCallBack_;
    ConnectionStatusCallBack connectionStatusCallBack_;
    WriteCompleteCallBack writeCompleteCallBack_;
    errorCallBack errorCallBack_;

    std::map<int, TcpConnectionPtr> TcpConnections_;
public:
    Server(int port, char *address);
    void setReadCompleteCallBack(const ReadCompleteCallBack &cb);
    void setClosingCallBack(ClosingCallBack cb);
    void setConnectionStatusCallBack(ConnectionStatusCallBack cb);
    void setWriteCompleteCallBack(WriteCompleteCallBack cb);
    void newConnection(int sockfd);
    void removeConnection(TcpConnectionPtr Connection);
    void start();
    ~Server();
};


#endif