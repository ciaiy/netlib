#include "Server.h"

void Server::newConnection(int sockfd) {
    TcpConnection *newconn = new TcpConnection(&loop_, sockfd);
    newconn->setClosingCallBack(defaultClosingCallBack);
    newconn->setConnectionStatusCallBack(defalutConnectionStatusCallBack);
    newconn->setErrorCallBack(defaultErrorCallBack);
    newconn->setReadCompleteCallBack(defaultReadCompleteCallBack);
    newconn->setWriteCompleteCallBack(defaultWriteCompleteCallBack);
    TcpConnections_[newconn->getSockfd()] = newconn;
}

void Server::start() {
    loop_.updateChannel(accpetor_.getChannel());
    loop_.loop();
}

Server::Server(int port, char *address) : accpetor_(&loop_, port, address) , loop_()
{
    accpetor_.setNewConnectionCallBack(std::bind(&Server::newConnection, this, std::placeholders::_1));
}

Server::~Server()
{
}
