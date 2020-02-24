#include "Server.h"

void Server::setClosingCallBack(ClosingCallBack cb) {
    closingCallBack_ = cb;
}

void Server::setReadCompleteCallBack(const ReadCompleteCallBack& cb) {
    readCompleteCallBack_ = cb;
}

void Server::setConnectionStatusCallBack(ConnectionStatusCallBack cb) {
    connectionStatusCallBack_ = cb;
}

void Server::setWriteCompleteCallBack(WriteCompleteCallBack cb) {
    writeCompleteCallBack_ = cb;
}

void Server::removeConnection(TcpConnectionPtr Connection)
{
    TcpConnections_.erase(Connection->getSockfd());
}

void Server::newConnection(int sockfd)
{
    if (TcpConnections_.end() != TcpConnections_.find(sockfd))
    {
        auto newcon = TcpConnections_[sockfd];
        static_cast<TcpConnectionPtr>(newcon)->renew();
    }else {
        Eventloop *thisloop = eventloopThreadpool_.getNextLoop();
        TcpConnectionPtr newconn = std::shared_ptr<TcpConnection>(new TcpConnection(thisloop, sockfd));
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
    loop_.loop();
}

Server::Server(int port, char *address) : accpetor_(&loop_, port, address), loop_(), eventloopThreadpool_(&loop_, string("baseloop"))
{
    accpetor_.setNewConnectionCallBack(std::bind(&Server::newConnection, this, std::placeholders::_1));
    readCompleteCallBack_ = defaultReadCompleteCallBack;
    closingCallBack_ = defaultClosingCallBack;
    connectionStatusCallBack_ = defalutConnectionStatusCallBack;
    writeCompleteCallBack_ = defaultWriteCompleteCallBack;
    errorCallBack_ = defaultErrorCallBack;
}

Server::~Server()
{
}
