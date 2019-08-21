#include "ServerSocket.h"
#include "SocketOps.h"

void ServerSocket::listen(int num)
{
    socketopt::Listen(sockfd_, 1000);
}

int ServerSocket::accecpt()
{
    int ret_value = socketopt::Accept(sockfd_, (struct sockaddr *)&myaddr_, &addrlen_);
    return ret_value;
}

void ServerSocket::setNonblock(bool set)
{
    socketopt::setNonblock(sockfd_, set);
}

void ServerSocket::setKeepAlive(bool set)
{
    if (!set)
    {
        socketopt::closeKeepAlive(sockfd_);
    }
}

void ServerSocket::setKeepAlive(int keepIdle, int keepInterval, int keepCount)
{
    socketopt::setKeepAlive(sockfd_, keepIdle, keepInterval, keepCount);
}