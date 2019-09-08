#include "Server.h"

void Server::newConnection(int sockfd) {
    log(DEBUG, "Server", __LINE__, "newConnection begin");
    printf("\t newconfd = %d\n", sockfd);
    TcpConnectionPtr newconn = std::shared_ptr<TcpConnection>(new TcpConnection(&loop_, sockfd));
    log(DEBUG, "Server", __LINE__, "test write begin");
    char msg[10] = "123"; 
    int ret_value = write(sockfd, msg, 10);
    if(ret_value == -1) {
        perror("write error");
        exit(-1);
    }
    log(DEBUG, "Server", __LINE__, "test write end");
    newconn->setClosingCallBack(std::bind(&defaultClosingCallBack, std::placeholders::_1));
    newconn->setConnectionStatusCallBack(std::bind(&defalutConnectionStatusCallBack, std::placeholders::_1));
    newconn->setErrorCallBack(std::bind(&defaultErrorCallBack));
    newconn->setReadCompleteCallBack(std::bind(&defaultReadCompleteCallBack, std::placeholders::_1));
    newconn->setWriteCompleteCallBack(std::bind(&defaultWriteCompleteCallBack, std::placeholders::_1));
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
