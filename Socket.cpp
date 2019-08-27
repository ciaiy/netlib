#include "Socket.h"
#include "SocketOps.h"

int Socket::getsockfd() {
    return sockfd_;
}

void Socket::listen(int num)
{
    socketopt::Listen(sockfd_, 1000);
}

int Socket::accept()
{
    int ret_value = socketopt::Accept(sockfd_, (struct sockaddr *)&myaddr_, &addrlen_);
    return ret_value;
}

void Socket::setNonblock(bool set)
{
    socketopt::setNonblock(sockfd_, set);
}

void Socket::setKeepAlive(bool set)
{
    if (!set)
    {
        socketopt::closeKeepAlive(sockfd_);
    }
}

void Socket::setKeepAlive(int keepIdle, int keepInterval, int keepCount)
{
    socketopt::setKeepAlive(sockfd_, keepIdle, keepInterval, keepCount);
}