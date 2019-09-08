#include "Server.h"

void Server::newConnection(int sockfd) {
    log(DEBUG, "Server", __LINE__, "newConnection begin");
    printf("\t newconfd = %d\n", sockfd);
    TcpConnection *newconn = new TcpConnection(&loop_, sockfd);
    log(DEBUG, "Server", __LINE__, "test write begin");
    char msg[10] = "123"; 
    int ret_value = write(sockfd, msg, 10);
    if(ret_value == -1) {
        perror("write error");
        exit(-1);
    }
    log(DEBUG, "Server", __LINE__, "test write end");
    newconn->setClosingCallBack(defaultClosingCallBack);
    newconn->setConnectionStatusCallBack(defalutConnectionStatusCallBack);
    newconn->setErrorCallBack(defaultErrorCallBack);
    newconn->setReadCompleteCallBack(defaultReadCompleteCallBack);
    newconn->setWriteCompleteCallBack(defaultWriteCompleteCallBack);
    TcpConnections_[newconn->getSockfd()] = newconn;
    log(DEBUG, "Server", __LINE__, "newConnection end");
}

void Server::start() {
    log(DEBUG, "Server", __LINE__, "start begin");
    loop_.loop();
    log(DEBUG, "Server", __LINE__, "start end");
}

Server::Server(int port, char *address) : accpetor_(&loop_, port, address), loop_()
{
    log(DEBUG, "Server", __LINE__, "constructor begin");
    accpetor_.setNewConnectionCallBack(std::bind(&Server::newConnection, this, std::placeholders::_1));
    log(DEBUG, "Server", __LINE__, "constructor end");
}

Server::~Server()
{
}
