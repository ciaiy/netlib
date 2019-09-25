#include "Server.h"

void Server::removeConnection(TcpConnectionPtr Connection)
{
    TcpConnections_.erase(Connection->getSockfd());
}

void Server::newConnection(int sockfd)
{
    log(INFO, "Server", __LINE__, "new Connection : ");
    std::cout << "sockfd : " << sockfd << std::endl;
    if (TcpConnections_.end() != TcpConnections_.find(sockfd))
    {
        auto newcon = TcpConnections_[sockfd];
        static_cast<TcpConnectionPtr>(newcon)->renew();
    }else {
        TcpConnectionPtr newconn = std::shared_ptr<TcpConnection>(new TcpConnection(&loop_, sockfd));
        newconn->setClosingCallBack(closingCallBack_);
        newconn->setConnectionStatusCallBack(connectionStatusCallBack_);
        newconn->setErrorCallBack(errorCallBack_);
        newconn->setReadCompleteCallBack(readCompleteCallBack_);
        newconn->setWriteCompleteCallBack(writeCompleteCallBack_);
        TcpConnections_[newconn->getSockfd()] = newconn;
    }
}

void Server::start()
{
    log(INFO, "Server", __LINE__, "start begin");
    loop_.loop();
    log(INFO, "Server", __LINE__, "start end");
}

Server::Server(int port, char *address) : accpetor_(&loop_, port, address), loop_()
{
    accpetor_.setNewConnectionCallBack(std::bind(&Server::newConnection, this, std::placeholders::_1));
    readCompleteCallBack_ = defaultReadCompleteCallBack;
    closingCallBack_ = defaultClosingCallBack;
    connectionStatusCallBack_ = defalutConnectionStatusCallBack;
    writeCompleteCallBack_ = defaultWriteCompleteCallBack;
    errorCallBack_ = defaultErrorCallBack;
}

// void setReadCompleteCallBack(ReadCompleteCallBack cb) {

// }

Server::~Server()
{
}
