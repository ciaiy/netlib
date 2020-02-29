#include "Socket.h"
#include "SocketOps.h"

string Socket::toString() {
    printf("Socket::toString\nmy sockfd : %d\n", sockfd_);
}

void Socket::Shutdown() {
    socketopt::Shutdown(sockfd_);
}

int Socket::write(char *buf, int len)
{
    return socketopt::write(sockfd_, buf, len);
}

void Socket::setReuseAddr(bool set)
{
    socketopt::setReuseAddr(sockfd_, set);
}

int Socket::getSockfd()
{
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
    socketopt::setKeepAlive(sockfd_, set);
}

Socket::Socket(int sockfd)
{
    sockfd_ = sockfd;
}

Socket::Socket(int port, string address) : port_(port), address(address)
{
    addrlen_ = sizeof(myaddr_);
    myaddr_.sin_family = AF_INET;
    myaddr_.sin_port= htons(port);

    int ret_value = inet_pton(AF_INET, address.c_str(), &(myaddr_.sin_addr.s_addr));
    if (ret_value == -1)
    {
        perror("inet_pton error");
        delete (this);
    }

    sockfd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd_ == -1)
    {
        perror("socket error");
        delete (this);
    }
    this->setReuseAddr(true);
    ret_value = bind(sockfd_, (struct sockaddr *)&myaddr_, addrlen_);
    if (ret_value == -1)
    {
        perror("bind error");
        delete (this);
    }
}

Socket::~Socket()
{
    close(sockfd_);
}