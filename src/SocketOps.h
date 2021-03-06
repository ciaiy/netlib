#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

namespace socketopt
{

int write(int sockfd, char *buf, int len) {
    printf("socketopt::write\n %s sockfd %d len;:%d\n", buf, sockfd, len);
    ::write(sockfd, buf, len);
}

void Shutdown(int sockfd) {
    shutdown(sockfd, SHUT_RDWR);
}

void setReuseAddr(int sockfd, bool set) {
    int optval = set ? 1 : 0;
    int ret_value = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if(ret_value == -1) {
        perror("set reuse addr error");
    }
}

void Listen(int sockfd, int num)
{
    int ret_value = listen(sockfd, num);
    if (ret_value == -1)
    {
        perror("listen error");
    }
}

void Bind(int sockfd, struct sockaddr *addr, socklen_t len)
{
    int ret_value = bind(sockfd, addr, len);
    if (ret_value == -1)
    {
        perror("binf error");
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *len)
{
    int ret_value = accept4(sockfd, addr, len, SOCK_NONBLOCK|SOCK_CLOEXEC);
    if (ret_value == -1)
    {
        perror("accept error");
    }
    return ret_value;
}

int Socket(int domin, int type, int protocol)
{
    int ret_value = socket(domin, type, protocol);
    if (ret_value == -1)
    {
        perror("socket error");
    }
    return ret_value;
}

void setNonblock(int sockfd_, bool set)
{
    int flag = fcntl(sockfd_, F_GETFL, 0);
    flag |= SOCK_NONBLOCK;
    if (set)
    {
        flag |= SOCK_NONBLOCK;
    }
    else
    {
        flag &= ~SOCK_NONBLOCK;
    }
    fcntl(sockfd_, F_SETFL, flag);
}

void setKeepAlive(int sockfd, bool set) {
    int optval = set ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
}

};     // namespace socketopt
#endif // !SOCKETOPS_H
